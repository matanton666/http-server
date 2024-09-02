 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "../include/socketHandler.h"
#include "../include/requestParser.h"
#include "../include/requestHandler.h"
#include "../include/helpers.h"


void* client_chat(void* client_data);
void remove_char_instances(char* s, char c);
void print_req(request_t* req);


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

    char buff[4096];

    
    memset(buff, 0, 4096);
    recv(cli_data.client_descriptor, buff, 4096, 0);
    // printf("recieved: %s", buff);

    remove_char_instances(buff, '\r');

    clock_t s = timer_start(); 
    request_t* req = parse_request(buff); // this function takes the most time to execute
    if (!req) {
        printf("invalid request, exiting");
        printf("\nerrno: %d\n", errno);
        return NULL;
    }

    // printf("\nreq type: %d\n", req->type);
    // printf("URL: %s\n", req->url->domain);
    // printf("data: \n%s\n", req->data);

    response_t* resp = handle_request(req);
    if (!resp) {
        printf("\nresponse not good, exeting");
        printf("\nerrno: %d", errno);
        free_request_t(req);
        resp = build_500();
    }

    char* to_send;
    unsigned long send_len = response_to_buff(resp, &to_send);
    send(cli_data.client_descriptor, to_send, send_len, 0);
    free(to_send);

    print_timer_end(req->url->domain, s);
    printf("\n");

    free_request_t(req);
    free_response(resp);

    close(cli_data.client_descriptor);

    free(client_data);
    return NULL;

    // todo: add 500 internal error
    // todo: add application/json for sending images and non text stuff (content type)
}


void print_req(request_t* req)
{
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
        hash_table_iter* iter = create_iterator(req->headers);
        do {
            printf("%s : %s\n",iter->curr->key, iter->curr->value);
            point_next(iter);
        }while (has_next(iter));
        free(iter);

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
}

void remove_char_instances(char* s, char c)
{
 
    int j, n = strlen(s);
    for (int i = j = 0; i < n; i++)
        if (s[i] != c)
            s[j++] = s[i];
 
    s[j] = '\0';
}