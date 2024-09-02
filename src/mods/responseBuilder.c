#include "../../include/responseBuilder.h"

const char* res_folder =  "../res";
const char* fourOfour = "/404.html";

// helper function that copies from src to dest and puts dest pointer to one character after end of copy
char* strcpycat(char* dest, char* src)
{
    int len = strlen(src);
    strncpy(dest, src, len);
    dest += len;
    return dest;
}

char* construct_file_path(const char* file_name, int name_len)
{
    // make sure file name is ../res/*
    char* file_path = malloc(strlen(res_folder) + name_len + 1);
    strcpy(file_path, res_folder);
    strncat(file_path, file_name, name_len);
    file_path[strlen(res_folder)+name_len] = '\0';

    return file_path;
}


unsigned long read_file(char* file_name, char** content)
{
    FILE* f = fopen(file_name, "rb");
    if (!f) {
        *content = NULL;
        return 0;
    }

    // get file len
    fseek(f, 0, SEEK_END);
    unsigned long len = ftell(f);
    rewind(f);
    // read to buff
    *content = malloc(len + 1);
    fread(*content, 1, len, f);
    (*content)[len] = '\0';

    return len;
}


// helper function to find out the length of a response if it were to be a complete string
int response_len(response_t* resp)
{
  int len = strlen("HTTP/1.x ");
    len += 4; // code
    len += strlen(resp->reason_phrase) + 2; // +2 for enter

    if (resp->headers) {
        hash_table_iter* iter = create_iterator(resp->headers);
        while (iter->curr != NULL){
            len += strlen(iter->curr->key);
            len += strlen(iter->curr->value);
            len += 4; // for ': ' and \r\n
            point_next(iter);
        }
        free(iter);
    }

    len += 2; // for \r\n
    len += resp->body_len;

    return len;
}





unsigned long response_to_buff(response_t* resp, char** buff)
{
    int len = response_len(resp);

    *buff = (char*)malloc(len+1);
    char* tmp = *buff;

    tmp = strcpycat(tmp, "HTTP/1.");
    *tmp = '0' + resp->version;
    tmp++;
    *tmp = ' ';
    tmp++;

    tmp = strcpycat(tmp, resp->status_code);
    *tmp = ' ';
    tmp++;

    tmp = strcpycat(tmp, resp->reason_phrase);
    strncpy(tmp, "\r\n", 2);
    tmp += 2;
    
    if (resp->headers) {

        hash_table_iter* iter = create_iterator(resp->headers);
        while(iter->curr != NULL){
            tmp = strcpycat(tmp, iter->curr->key);
            strncpy(tmp, ": ", 2);
            tmp += 2;

            tmp = strcpycat(tmp, iter->curr->value);
            strncpy(tmp, "\r\n", 2);
            tmp += 2;

            point_next(iter);
        } 
        free(iter);
    }

    tmp = strcpycat(tmp, "\r\n");
    if (resp->body) {
        memcpy(tmp, resp->body, resp->body_len);
        tmp += resp->body_len;
    }
    *tmp = '\0';

    return len + 1;
}



response_t* build_response(int status_code, hash_table_t* headers, char* body, unsigned long body_len)
{
    response_t* resp = malloc(sizeof(response_t));
    resp->version = V_ONE;

    char* reason;
    switch (status_code) {
        case 200:
            strncpy(resp->status_code, "200\0", 4);
            reason = "OK";
            break;

        case 404:
            strncpy(resp->status_code, "404\0", 4);
            reason = "Not Found";
            break;

        case 500:
            strncpy(resp->status_code, "500\0", 4);
            reason = "Internal Server Error";
            break;

        case 302:
            strncpy(resp->status_code, "302\0", 4);
            reason = "Found";
            break;

        case 403:
            strncpy(resp->status_code, "403\0", 4);
            reason = "Forbidden";
            break;

        case 401:
            strncpy(resp->status_code, "401\0", 4);
            reason = "Unauthorized";
            break;

        default:
            strncpy(resp->status_code, "000\0", 4);
            reason = "Unknown Status";
            break;
    }
    resp->reason_phrase = strdup(reason);
    resp->headers = headers;

    if (body) {
        resp->body = malloc(body_len);
        memcpy(resp->body, body, body_len);
        resp->body_len = body_len;

        // add content length header
        if (!resp->headers) {
            resp->headers = create_table();
        }
        char* len_str = malloc(21); // max digit lenth of unsigned long
        sprintf(len_str, "%lu", body_len); // convert from char to int
        insert(resp->headers, "Content-Length", len_str);
        free(len_str);
    }
    else {
        resp->body = NULL;
        resp->body_len = body_len;
    }

    return resp;
}


void free_response(response_t* resp)
{
    if (resp->reason_phrase) {
        free(resp->reason_phrase);
    }
    if (resp->headers) {
        free_table(resp->headers);
    }
    if (resp->body && *(resp->body)) {
            free(resp->body);
    }
    free(resp);
}




response_t* build_404()
{
    char* msg = NULL;
    char* file_name = construct_file_path(fourOfour, strlen(fourOfour));
    unsigned long len = read_file(file_name, &msg);
    printf("\nFILENAME: %s", file_name);
    free(file_name);
    if (!msg) {
        msg = strdup("<h1> 404 page not found <h1>");
        len = strlen(msg);
    }
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");

    response_t* resp = build_response(404, headers, msg, len);
    free(msg);
    return resp;
}


response_t* build_302(const char* new_path)
{
    char* path = strdup(new_path);
    if (path[0] != '/') {
        free(path);

        path = malloc(strlen(new_path) + 2);
        path[0] = '\0';
        strcat(path, "/");
        strncat(path, new_path, strlen(new_path));
    }

    hash_table_t* headers = create_table();
    insert(headers, "Location", new_path);
    response_t* resp = build_response(302, headers, NULL, 0);

    free(path);
    return resp;
}


response_t* build_500()
{
    char* msg = "<html> \
<head><title>500 Internal Server Error</title></head> \
<body><h1>500 Internal Server Error<h1>  \
Something went wrong.</body> \
</html>";

    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");
    return build_response(500, headers, msg, strlen(msg));
}