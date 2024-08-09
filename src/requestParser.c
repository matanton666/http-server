
#include "include/requestParser.h"

http_version get_req_http_version(char *req)
{
    char search[] = "HTTP/1.";
    int curr = 0;

    while (curr <= strlen(req) && req[curr] != '\n') {
        if (req[curr] == search[0]){
            // validate that there is http/1.
            if (strncmp(search, req + curr, strlen(search)) == 0) {
                curr += strlen(search);
                if (*(req + curr + 1) != '\n') { // search for enter after 
                    return V_INVALID;
                }

                switch (*(req + curr)) {
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
        }
        curr++;
    }
    return V_INVALID;
}


request_type get_req_type(char *req)
{
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
    req = strdup(req);
    char* pos = req;

    while (*pos != '\n') {  // skip request line
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


    free(req);
    return headers;
}




