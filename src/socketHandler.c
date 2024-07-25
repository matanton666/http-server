#include "include/socketHandler.h"
#include <pthread.h>



int create_local_socket(uint16_t port)
{
    struct sockaddr_in server_addr = {
    AF_INET, // socket for internet ipv4
    htons(port), // convert port to network endian format
    {INADDR_ANY}, // incoming address can be any adress
    .sin_zero[7] = '\0'
    };

    int soc_descriptor, status, tmp = 1;


    soc_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    check_error(soc_descriptor, "could not create new socket");

    // set address of the socket to be 127.0.0.1 and available to reuse
    status = setsockopt(soc_descriptor, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
    check_error(status, "could not set socket options");

    status = bind(soc_descriptor, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    check_error(status, "could not bind address");

    return soc_descriptor;
}


int accept_loop(int socket_desc, void* (*client_handler)(void*))
{
    struct sockaddr_in tmp_client;
    unsigned int addr_len = sizeof(tmp_client);
    
    int conn_amount = 5; // amount of concurrent connections
    check_error(listen(socket_desc, conn_amount), "could not listen on socket port");

    while (true) {
        // accept client connection 
        int client_desc = accept(socket_desc, (struct sockaddr*)&tmp_client, &addr_len);

        if (client_desc < 0)
        {
            printf("couldent accept client");
            continue;
        }

        client_data_t* data = malloc(sizeof(client_data_t));
        data->client_descriptor = client_desc;
        data->server_descriptor = socket_desc;
        memcpy(&(data->client_addr), &tmp_client, sizeof(struct sockaddr));

        // client_handler(*data);

        pthread_t thread;
        int status = pthread_create(&thread, NULL, client_handler, (void*)data );
        if (status < 0 || pthread_detach(thread) < 0) 
        {
            printf("count not create thread for client");
            free(data);
        }
        printf("client connected: %d", client_desc);

        sleep(1);


        // todo:  make tests for functions

    }
    return 0;
}


void check_error(int return_code, const char* msg)
{
    if (return_code < 0)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}