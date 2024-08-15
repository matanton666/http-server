#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H


#include "socketHandler.h"
#include "hashTable.h"

#include <string.h>
#include <stdio.h>
#include <regex.h>




typedef enum request_type_t {
    REQ_INVALID,
    REQ_GET,
    REQ_POST,
    REQ_PUT,
    REQ_DELETE
}request_type_t;

typedef enum http_version_t {
    V_ZERO,
    V_ONE,
    V_TWO,
    V_INVALID
}http_version_t;

// structured in a way so that domain points to a continues string untill the end of query
typedef struct url_t {
    char* domain;
    char* path;
    char* query;

    short domain_len;
    short path_len;
}url_t;

typedef struct request_t {
    request_type_t type;
    http_version_t version;
    url_t* url;
    hash_table_t* headers;
    char* data;
}request_t;


// validates, and parses http request
// returns null if error or not valid
request_t* parse_request(char* req);

// only validates that the syntax is correct
// does not check if headers correct or match request type or anything like that
int validate_req_syntax(char* req);


// expects valid http request
http_version_t get_req_http_version(char* req);

// expecst valid http request
request_type_t get_req_type(char* req);


// expects a valid http request
// returns null on invalid hedaders or on error
hash_table_t* parse_req_headers(char* req);


// expects a valid http request, mallocs new url_t
// returns empty url if there are errors
url_t* parse_req_url(char* req, hash_table_t* headers);


void free_request_t(request_t* request);


#endif