#include "../../include/requestParser.h"


request_t* parse_request(char* req)
{
    if (!validate_req_syntax(req)) {
        errno = REQ_SYNTAX_ERR;
        return NULL;
    }
    
    request_t* desir_req = (request_t*)malloc(sizeof(request_t)); // deserialized request

    desir_req->type = get_req_type(req);
    desir_req->version = get_req_http_version(req);

    hash_table_t* headers = parse_req_headers(req);
    if (!headers) {
        free(desir_req);
        errno = PARSE_HEADERS_ERR;
        return NULL;
    }

    url_t* url = parse_req_url(req, headers);
    if (!url) {
        free(desir_req);
        free_table(headers);
        errno = URL_PARSE_ERR;
        return NULL;
    }

    desir_req->headers = headers;
    desir_req->url = url;

    char* data = strstr(req, "\n\n");
    if (data && data[2] != '\0') { // contains data
        data += 2; // skip 2 \n
        desir_req->data = (char*)malloc(strlen(data) + 1);
        strncpy(desir_req->data, data, strlen(data));
        desir_req->data[strlen(data)] = '\0';
    }
    else {
        desir_req->data = NULL;
    }

    return desir_req;
}

void free_request_t(request_t* request)
{
    if (request->headers) free_table(request->headers);
    if (request->url) free(request->url);
    if (request->data) free(request->data);
    free(request);
}




http_version_t get_req_http_version(char *req)
{

    char search[] = "HTTP/1.";
    
    if (!req || strlen(req) <= strlen(search)+1) return V_INVALID; // make sure req is at least the right size
    
    char* http = strstr(req, search);
    if (!http) return V_INVALID;

    http += strlen(search);
    if (*(http) != '\0')
    {
        switch (*(http)) {
            case '0':
                return V_ZERO;
            case '1':
                return V_ONE;
            case '2':
                return V_TWO;
            default:
                return V_INVALID;
        }
    }
 
    return V_INVALID;
}


request_type_t get_req_type(char *req)
{
    if (!req) return REQ_INVALID;

    while (*req == ' ') {req++;}  // remove trailing whitespaces

    if (strncmp("GET", req, 3) == 0) {
        return REQ_GET;
    }
    else if (strncmp("POST", req, 4) == 0) {
        return REQ_POST;
    }
    else if (strncmp("DELETE", req, 6) == 0) {
        return REQ_DELETE;
    }
    else if (strncmp("PUT", req, 3) == 0) {
        return REQ_PUT;
    }
    return REQ_INVALID;
}



hash_table_t* parse_req_headers(char* req)
{

    hash_table_t* headers = create_table();
    if (!headers || !req || strlen(req) == 0) {
        errno = -1;
        return NULL;
    } 

    char* tmp = strdup(req);
    char* pos = tmp;

    while (*pos != '\n') {  // skip first request line
        pos++;
    }
    pos++;

    char* headers_end = strstr(pos, "\n\n");
    if (headers_end == NULL) {
        headers_end = pos + strlen(pos);
    }
    else {
        headers_end++;
    }

    while ((*pos && *(pos+1)) && pos != headers_end && pos+1 != headers_end) { // untill get to data part
        // isolate key
        char* colon = strstr(pos, ": ");
        if (!colon) return NULL;
        *colon = '\0';
        char* key = pos;

        // isolate value
        char* value = colon + 2; // skip colon and space
        pos = value;
        pos = strchr(pos, '\n');
        if (!pos || !value) return NULL;
        *pos = '\0';

        insert(headers, key, value);
        pos++;
    }


    free(tmp);
    return headers;
}



int validate_req_syntax(char* req)
{
    regex_t regex   ;
    char msgbuf[100];
    int reti;
    
    if (!req || !(*req)) return false;

    // Sufficiently large buffer for the pattern
    char pattern[120] = "^(GET|POST|PUT|DELETE|HEAD|OPTIONS|TRACE|CONNECT) (/|/[^ ]+) HTTP/1\.[0-9]\n([A-Za-z0-9-]+: .+\n)*$";

    char *has_body = strstr(req, "\n\n");
    if (has_body) {
        strncpy(pattern, "^(GET|POST|PUT|DELETE|HEAD|OPTIONS|TRACE|CONNECT) (/|/[^ ]+) HTTP/1\.[0-9]\n([A-Za-z0-9-]+: .+\n)*\n(.*\n*)*", sizeof(pattern) - 1);
        pattern[sizeof(pattern) - 1] = '\0';  // Ensure null-termination
    }
    // printf("\n%s\n", pattern);


    // Compile regex
    reti = regcomp(&regex, pattern, REG_EXTENDED | REG_NEWLINE);
    if (reti != 0) {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        printf("Regex error compiling '%s': %s\n", pattern, msgbuf);
        return false;
    }

    // Execute regex
    reti = regexec(&regex, req, 0, NULL, 0);
    if (!reti) {
        regfree(&regex);
        return true;
    } 
    else if (reti == REG_NOMATCH) {
        // printf("HTTP request is not valid.\n");
        regfree(&regex);
        return false;
    } 
    else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        regfree(&regex);
        return false;
    }
}


url_t* parse_req_url(char* req, hash_table_t* headers)
{
    url_t* url = NULL; 

    if (!headers) return NULL;
    char* host = search(headers, "Host");
    if (!host) return NULL;

    url = (url_t*)malloc(sizeof(url_t));

    // find request path
    char* path = strchr(req, '/');
    if (!path) {
        free(url);
        return NULL;
    }
    char* path_end = strstr(req, " HTTP/1.");
    int path_len = path_end - path;

    // create one big string for entire url and make different parts point to certain portions
    url->domain = (char*)malloc(strlen(host) + path_len + 1); 
    strcpy(url->domain, host);
    url->domain_len = strlen(host);

    url->path = url->domain + url->domain_len; 
    strncpy(url->path, path, path_len);
    url->path[path_len] = '\0';

    url->query = strchr(url->path, '?'); // will put null if not found
    if (!url->query){
        url->query = url->path + path_len; // point query to end of string
        url->path_len = url->query - url->path;
    } 
    else {
        url->query += 1; // skip '?' before actual querys
        url->path_len = url->query - url->path - 1; // -1 to accomidate for skipping '?'
    }
    
    return url;
}

