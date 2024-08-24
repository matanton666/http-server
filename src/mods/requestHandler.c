#include "../../include/requestHandler.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



response_t* handle_get(request_t* req)
{
    char* file_name = strndup(req->url->path + 1, req->url->path_len - 1);

    if (strlen(file_name) == 0) { 
        // todo: send redirect to index.html
        file_name = "index.html";
    }

    FILE* f = fopen(file_name, "rb");
    if (!f) {
        // todo: return 404
        free(file_name);
        return NULL;
    }
    free(file_name);

    // get file len
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);
    // read to buff
    char* buff = malloc(len + 1);
    fread(buff, 1, len, f);
    buff[len] = '\0';

    hash_table_t* headers = create_table();
    char* len_str = malloc(10);
    sprintf(len_str, "%d", len+1);
    
    insert(headers, "Content-Length", len_str);
    insert(headers, "Content-Type", "text/html");
    insert(headers, "Connection", "keep-alive");

    return build_response(200, headers, buff);
}




response_t* handle_request(request_t* req)
{
    switch (req->type) {
        case REQ_GET:
            return handle_get(req);
        default:
            return NULL;
    }
    return NULL;
}



