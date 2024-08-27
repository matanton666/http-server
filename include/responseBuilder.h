#ifndef RESPONSEBUILDER_H
#define RESPONSEBUILDER_H

#include "hashTable.h"
#include "requestParser.h"
#include <string.h>
#include <stdio.h>

/*
HTTP/1.1 200 OK
Date: Thu, 21 Aug 2024 14:28:00 GMT
Content-Type: text/html; charset=UTF-8
Content-Length: 138
Connection: keep-alive

<html>
<head><title>Example Page</title></head>
<body>Hello, World!</body>
</html>



HTTP/1.1 301 Moved Permanently
Location: https://www.example.com/
Content-Length: 0



HTTP/1.1 404 Not Found
Date: Thu, 21 Aug 2024 14:28:00 GMT
Content-Type: text/html
Content-Length: 93

<html>
<head><title>404 Not Found</title></head>
<body>404 Not Found: The requested resource could not be found.</body>
</html>




HTTP/1.1 500 Internal Server Error
Date: Thu, 21 Aug 2024 14:28:00 GMT
Content-Type: text/html
Content-Length: 80

<html>
<head><title>500 Internal Server Error</title></head>
<body>Something went wrong.</body>
</html>



HTTP/1.1 302 Found
Location: https://www.example.com/new-page
Content-Length: 0



*/




typedef struct response_t
{
    http_version_t version;
    char status_code[4];
    char* reason_phrase;
    hash_table_t* headers;
    char* body;
}response_t;

// builds response object and automatically adds the content length header
response_t* build_response(int status_code, hash_table_t* headers, char* body);

char* response_to_str(response_t* resp);

void free_response(response_t* resp);


char* read_file(char* file_name);


// not found
response_t* build_404();
// internal server error
response_t* build_500();
// redirect
response_t* build_302();


#endif