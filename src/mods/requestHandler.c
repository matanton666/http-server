#include "../../include/requestHandler.h"

const char* res_folder =  "../res\0";


response_t* handle_get(request_t* req)
{
    // make sure file name is ../res/*
    char* file_name = malloc(strlen(res_folder) + req->url->path_len);
    strcpy(file_name, res_folder);
    strncat(file_name, req->url->path, req->url->path_len);

    if (strlen(file_name) == strlen(res_folder)) {  // get '/'
        // todo: redirect
        free(file_name);
        return NULL;
    }

    char* buff = read_file(file_name);
    if (!buff) {
        free(file_name);
        return build_404();
    }

    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");
    insert(headers, "Connection", "keep-alive");

    response_t* resp = build_response(200, headers, buff);
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



