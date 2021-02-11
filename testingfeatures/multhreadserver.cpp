#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>

#define TRUE 1
#define FALSE 0
#define PORT 8888

int main(int argc , char *argv[])   
{
    int opt = TRUE;
    int master_socket, addrlen, new_socket, client_socket[30], max_clients = 30, activity, i, valread, sd, sockets_number = 0;
    int max_sd;
    struct sockaddr_in address;
    fd_set readfds;

    char buffer[1025];
    char *message = "Message!!!";

    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }
    master_socket = socket(AF_INET , SOCK_STREAM , 0);
    setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    bind(master_socket, (struct sockaddr *)&address, sizeof(address));
    printf("Listener on port %d \n", PORT);
    listen(master_socket, 3);
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
    while(TRUE)
    {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
        for ( i = 0 ; i < max_clients ; i++)
        {
            sd = client_socket[i];
            if(sd > 0)
            {
                FD_SET( sd , &readfds);
            }
            if(sd > max_sd)
            {
                max_sd = sd;
            }
        }
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if (FD_ISSET(master_socket, &readfds))
        {
            new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            send(new_socket, message, strlen(message), 0);
            puts("Welcome message sent successfully");
            for (i = 0; i < max_clients; i++)
            {
                if(client_socket[i] == 0)
                {
                    sockets_number++;
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                    break;
                }
            }
        }
        for(int i = 0; i < sockets_number; i++){
            write(client_socket[i], &i, sizeof(i));
            std::cout << i << "\n";
        }
    }
    std::cout << "Hello\n";
    return 0;
}
