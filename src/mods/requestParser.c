
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
    char* tmp = strdup(req);
    char* pos = tmp;

    while (*pos != '\n') {  // skip first request line
        pos++;
    }
    pos++;

    while (*pos && *(pos++) && *pos != '\n' && *(pos++) != '\n') { // untill get to data part
        // isolate key
        char* colon = strstr(pos, ": ");
        *colon = '\0';
        char* key = pos;

        // isolate value
        char* value = colon + 2;
        pos = strchr(pos, '\n');
        *pos = '\0';

        insert(headers, key, value);
        pos++;
    }


    free(tmp);
    return headers;
}




