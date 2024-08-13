
#include "../../include/requestParser.h"


http_version get_req_http_version(char *req)
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


request_type get_req_type(char *req)
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



HashTable* parse_req_headers(char* req)
{

    HashTable* headers = create_table();
    if (!headers || !req) return NULL;

    char* tmp = strdup(req);
    char* pos = tmp;

    while (*pos != '\n') {  // skip first request line
        pos++;
    }
    pos++;

    while ((*pos && *(pos+1)) && (pos && pos+1) && (*pos != '\n' && *(pos+1) != '\n')) { // untill get to data part
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
    regex_t regex;
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


url_t parse_req_url(char* req)
{
    // todo: implement
}