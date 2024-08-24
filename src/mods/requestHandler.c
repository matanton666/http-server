#include "../../include/requestHandler.h"
#include <string.h>

char* res_file =  "../res\0";


response_t* handle_get(request_t* req)
{
    // make sure file name is ../res/*
    char* file_name = malloc(strlen(res_file) + req->url->path_len);
    strcpy(file_name, res_file);
    strncat(file_name, req->url->path, req->url->path_len);

    if (strlen(file_name) == strlen(res_file)) { 
        // todo: redirect
        strcat(file_name, "/index.html");
    }

    char* buff = read_file(file_name);
    if (!buff) {
        free(file_name);
        return build_404();
    }

    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");
    insert(headers, "Connection", "close");

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



