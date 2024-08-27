#include "../../include/responseBuilder.h"
#include <time.h>



// helper function that copies from src to dest and puts dest pointer to one character after end of copy
char* strcpycat(char* dest, char* src)
{
    int len = strlen(src);
    strncpy(dest, src, len);
    dest += len;
    return dest;
}


char* read_file(char* file_name)
{
    FILE* f = fopen(file_name, "rb");
    if (!f) {
        return NULL;
    }

    // get file len
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    rewind(f);
    // read to buff
    char* buff = malloc(len + 1);
    fread(buff, 1, len, f);
    buff[len] = '\0';

    return buff;
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
    if (resp->body != NULL) {
        len += strlen(resp->body);
    }

    return len;
}





char* response_to_str(response_t* resp)
{
    int len = response_len(resp);


    char* response = (char*)malloc(len+1);
    char* tmp = response;

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
        tmp = strcpycat(tmp, resp->body);
    }
    *tmp = '\0';
    return response;
}



response_t* build_response(int status_code, hash_table_t* headers, char* body)
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
        resp->body = strdup(body);
        // add content length header
        if (!resp->headers) {
            resp->headers = create_table();
        }
        char* len_str = malloc(21); // max digit lenth of unsigned long
        sprintf(len_str, "%lu", strlen(resp->body)); // convert from char to int
        //                          todo: might need to remove ^
        insert(resp->headers, "Content-Length", len_str);
        free(len_str);
    }
    else {
        resp->body = NULL;
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
    char* msg = read_file("404.html");
    if (!msg) {
        msg = strdup("<h1> 404 page not found <h1>");
    }
    hash_table_t* headers = create_table();
    insert(headers, "Content-Type", "text/html");

    response_t* resp = build_response(404, headers, msg);
    free(msg);
    return resp;
}

