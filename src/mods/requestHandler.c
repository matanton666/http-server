#include "../../include/requestHandler.h"
#include <string.h>
#include <time.h>

const char* res_folder =  "../res";


response_t* handle_get(request_t* req)
{
    // make sure file name is ../res/*
    char* file_name = malloc(strlen(res_folder) + req->url->path_len + 1);
    strcpy(file_name, res_folder);
    strncat(file_name, req->url->path, req->url->path_len);
    file_name[strlen(res_folder)+req->url->path_len] = '\0';

    if (strlen(file_name) == strlen(res_folder)) {  // get '/'
        // todo: redirect
        free(file_name);
        return NULL;
    }

    char* buff = NULL;
    unsigned long len = read_file(file_name, &buff);
    if (!buff) {
        free(file_name);
        return build_404();
    }
    // todo: update code to handle file len
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");
    insert(headers, "Connection", "keep-alive");

    response_t* resp = build_response(200, headers, buff, len);
    free(buff);
    return resp;
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



