#include "../../include/socketHandler.h"

int bind_local_socket(uint16_t port)
{
    struct sockaddr_in server_addr = {
    AF_INET, // socket for internet ipv4
    htons(port), // convert port to network endian format
    {INADDR_ANY}, // incoming address can be any adress
    .sin_zero[7] = '\0'
    };

    int soc_descriptor, status, tmp = 1;


    soc_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_AZ(soc_descriptor, "could not create new socket", -1);

    // set address of the socket to be 127.0.0.1 and available to reuse
    status = setsockopt(soc_descriptor, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
    CHECK_AZ(status, "could not set socket options", -1);

    status = bind(soc_descriptor, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    CHECK_AZ(status, "could not bind address", -1);

    return soc_descriptor;
}

int socket_start_listen(int socket_desc, int max_conns)
{
    if (max_conns <= 0) {
        return -1;
    }
    CHECK_AZ(listen(socket_desc, max_conns), "could not listen on socket port", -1);
    return 0;
}

int accept_loop(int socket_desc, void* (*client_handler)(void*))
{
    struct sockaddr_in tmp_client;
    unsigned int addr_len = sizeof(tmp_client);


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


        pthread_t thread;
        int status = pthread_create(&thread, 
         NULL,
         client_handler, 
         (void*)data
        );
        if (status < 0 || pthread_detach(thread) < 0) 
        {
            printf("count not create thread for client");
            free(data);
        }

    }
    return 0;
}
