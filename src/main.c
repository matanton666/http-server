 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "../include/socketHandler.h"
#include "../include/requestParser.h"


void* client_chat(void* client_data);


int main(int argc, char *argv[])
{
    int soc_descript = bind_local_socket(1234);
    CHECK_AZ(soc_descript, "socket descript creation", -1);
    CHECK_AZ(socket_start_listen(soc_descript, 5), "listen failed", -22);
    accept_loop(soc_descript, (void*)client_chat);

    return 0;
}


void* client_chat(void* client_data)
{
    client_data_t cli_data = *((client_data_t*) client_data);

    char buff[2048];
    printf("\n desc: %d", cli_data.client_descriptor);


    while (true) {
        memset(buff, 0, 256);
        recv(cli_data.client_descriptor, buff, 256, 0);
        printf("%s\nsending: ", buff);

        fgets(buff, 256, stdin);

        printf("%s\n", buff);
        send(cli_data.client_descriptor, buff, 256, 0);
        printf("client: ");
    }
}

