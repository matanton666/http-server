 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "../include/socketHandler.h"
#include "../include/requestParser.h"


void* client_chat(void* client_data);
void remove_char(char* s, char c);


int main(int argc, char *argv[])
{
    printf("started server \n");
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
    memset(buff, 0, 2048);
    recv(cli_data.client_descriptor, buff, 2048, 0);
    printf("recieved: %s", buff);

    remove_char(buff, '\r');

    request_t* req = parse_request(buff);
    if (req != NULL) {
        printf("Request Type: ");
        switch (req->type) {
            case REQ_GET:
                printf("GET\n");
                break;
            case REQ_POST:
                printf("POST\n");
                break;
            case REQ_PUT:
                printf("PUT\n");
                break;
            case REQ_DELETE:
                printf("DELETE\n");
                break;
            default:
                printf("Invalid\n");
                break;
        }

        printf("HTTP Version: ");
        switch (req->version) {
            case V_ZERO:
                printf("HTTP/1.0\n");
                break;
            case V_ONE:
                printf("HTTP/1.1\n");
                break;
            default:
                printf("Invalid\n");
                break;
        }

        printf("URL: %s\n", req->url->domain);

        printf("Headers:\n");
        printf("%s : %s\n","Host", search(req->headers, "Host"));

        if (req->data != NULL) {
            printf("Data:\n%s\n", req->data);
        }
        else {
            printf("No data\n");
        }

        free_request_t(req);
    }
    else {
        printf("Invalid request\n");
    }




    // while (true) {
    //     memset(buff, 0, 256);
    //     recv(cli_data.client_descriptor, buff, 256, 0);
    //     printf("%s\nsending: ", buff);

    //     fgets(buff, 256, stdin);

    //     printf("%s\n", buff);
    //     send(cli_data.client_descriptor, buff, 256, 0);
    //     printf("client: ");
    // }

    free(client_data);
    return NULL;
}


void remove_char(char* s, char c)
{
 
    int j, n = strlen(s);
    for (int i = j = 0; i < n; i++)
        if (s[i] != c)
            s[j++] = s[i];
 
    s[j] = '\0';
}