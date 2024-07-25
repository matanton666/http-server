
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
  
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    int soc_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (soc_descriptor < 0) {return soc_descriptor;}

    int reuseaddr = 1;
    int soc_options = setsockopt(soc_descriptor, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    if (soc_options < 0) {return soc_options;}

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2222); // convert endian format from regular to network type
    server_addr.sin_addr.s_addr = INADDR_ANY; // any address can connect to sever
    server_addr.sin_zero[7] = '\0';
    int soc_bind = bind(soc_descriptor, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

    if (soc_bind < 0) 
    {
        perror("bind failed");
        return soc_bind;
    }

    int list = listen(soc_descriptor, 3);
    if (list < 0)
    {
        perror("listen failed");
        return list;
    }


    struct sockaddr_in client_addr;
    unsigned int addr_len = sizeof(client_addr);
    int client_soc = accept(soc_descriptor, (struct sockaddr*)&client_addr, &addr_len);

    if (client_soc < 0) 
    {
        perror("accept failed");
        return client_soc;
    }


    char buff[40];
    recv(client_soc, buff, 40, 0);

    printf("%s", buff);
    close(client_soc); 

    return 0;
}