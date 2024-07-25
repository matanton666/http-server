#ifndef SOCKETHANDER_H
#define SOCKETHANDER_H


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>  
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <time.h>


typedef struct client_data_t
{
    int client_descriptor;
    int server_descriptor;
    struct sockaddr client_addr;
}client_data_t;

// creates and binds socket on selected port for localhost 
// returns the socket descriptor
int create_local_socket(uint16_t port);

// using a created socket to make an accept loop that will send clients
// to seaprate threads using callback function
int accept_loop(int socket_desc, void* (*client_handler)(void*) );

void check_error(int return_code, const char* msg);

#endif