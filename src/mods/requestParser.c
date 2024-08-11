
#include "../../include/requestParser.h"
#include <stdio.h>


http_version get_req_http_version(char *req)
{

    char search[] = "HTTP/1.";
    printf("asdfasdfasdfsdff");
    
    if (!req || strlen(req) <= strlen(search)+1) return V_INVALID; // make sure req is at least the right size
    
    char* http = strstr(req, search);
    http += strlen(search);
    if (*(http + 1) != '\0')
    {
        printf("***************** %c ************", *(http + 1) );
        switch (*(http+1)) {
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


    // int curr = 0;

    // while (curr < strlen(req) && req[curr] != '\n') {
    //     if (req[curr] == search[0]){
    //         // validate that there is http/1.
    //         if (strncmp(search, req + curr, strlen(search)) == 0) {
    //             curr += strlen(search);
    //             if (*(req + curr + 1) != '\n') { // search for enter after 
    //                 return V_INVALID;
    //             }

       
    //         }
    //     }
    //     curr++;
    // }
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




