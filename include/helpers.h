#ifndef HELPERS_H
#define HELPERS_H

#include <time.h>
#include <stdio.h>
#include <errno.h>


enum errors {
    REQ_SYNTAX_ERR,
    PARSE_HEADERS_ERR,
    URL_PARSE_ERR,
    CREATE_TABLE_ERR,

};

clock_t timer_start();
void print_timer_end(const char* msg, clock_t start);

#endif