#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H


#include "socketHandler.h"
#include <strings.h>




typedef enum request_type {
    GET,
    POST,
    PUT,
    DELETE
}request_type;

typedef enum http_version {
    V_ZERO,
    V_ONE,
    V_TWO,
    V_INVALID
}http_version;


typedef struct url_t {
    char* subdomain;
    char* domain;
    char* path;
    char* query;
}url_t;

typedef struct request_t {
    http_version version;
    request_type req_type;
    url_t url;
    char* headers; // todo: create seprate struct or enum or list or something for this
    char* data;
}request_t;



// parse http request:
/*
- HTTP version
- request type (get, post...)
- url
- headers
- data




GET / HTTP/1.1
Host: 127.0.0.1:1234
Connection: keep-alive
sec-ch-ua: "Not/A)Brand";v="8", "Chromium";v="126", "Google Chrome";v="126", "Vivaldi";v="6.8"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"
DNT: 1
Upgrade-Insecure-Requests: 1
User-Agen����n
*/




http_version get_req_http_version(char* req);
request_type get_req_type(char* req);
url_t parse_req_url(char* req);


#endif