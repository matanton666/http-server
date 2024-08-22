#include "../../include/responseBuilder.h"
#include <stdio.h>
#include <time.h>


// helper function that copies from src to dest and puts dest pointer to one character after end of copy
char* strcpycat(char* dest, char* src)
{
    int len = strlen(src);
    strncpy(dest, src, len);
    dest += len;
    return dest;
}

// helper function to find out the length of a response if it were to be a complete string
int response_len(response_t* resp)
{
  int len = strlen("HTTP/1.x ");
    len += 4; // code
    len += strlen(resp->reason_phrase + 2); // +2 for enter

    hash_table_iter* iter = create_iterator(resp->headers);
    while (iter->curr != NULL){
        len += strlen(iter->curr->key);
        len += strlen(iter->curr->value);
        len += 4; // for ': ' and \r\n
        point_next(iter);
    }
    free(iter);

    len += 2; // for \r\n\r\n
    if (resp->body) {
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
    *tmp = resp->version + '0';
    tmp++;
    *tmp = ' ';
    tmp++;

    tmp = strcpycat(tmp, resp->status_code);
    *tmp = ' ';
    tmp++;

    tmp = strcpycat(tmp, resp->reason_phrase);
    strncpy(tmp, "\r\n", 2);
    tmp += 2;

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
    resp->reason_phrase = malloc(strlen(reason) + 1);
    strncpy(resp->reason_phrase, reason, strlen(reason) + 1);
    resp->reason_phrase[strlen(reason) + 1] = '\0';

    resp->headers = headers;
    resp->body = body;

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
    else if (resp->body) {
        free(resp->body);
    }
    free(resp);
}