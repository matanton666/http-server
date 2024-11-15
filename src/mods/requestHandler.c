#include "../../include/requestHandler.h"

const char* index_file = "index.html";

response_t* handle_get(request_t* req)
{
    if (req->url->path_len == 1) {  // get '/' : return redirect to index.html
        response_t* resp = build_302(index_file);
        return resp;
    }

    char* file_name = construct_file_path(req->url->path, req->url->path_len); 

    char* buff = NULL;
    unsigned long len = read_file(file_name, &buff);
    if (!buff) {
        free(file_name);
        return build_404();
    }
    // todo: create function for figuring out content type


    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", identify_content_type(file_name));
    insert(headers, "Connection", "close");

    response_t* resp = build_response(200, headers, buff, len);
    free(file_name);
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



