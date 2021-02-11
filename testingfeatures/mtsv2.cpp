#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <iostream>

char client_message[2000];
char buffer[1024];

void * socketThread(void *arg)
{
    int newSocket = *((int *)arg);
    while(true){
        int myint;
        for(int i = 0; i < 1000; i++){  
            read(newSocket, &myint, sizeof(myint));
            myint++;
            write(newSocket, &myint, sizeof(myint));
            std::cout << myint << '\n';
        }
    }
    close(newSocket);
    pthread_exit(NULL);
}

int main(){
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    listen(serverSocket,50);
    pthread_t tid[60];
    int i = 0;
    while(1)
    {
        addr_size = sizeof(serverStorage);
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        pthread_create(&tid[i++], NULL, socketThread, &newSocket);
        if( i >= 50)
        {
            i = 0;
            while(i < 50)
            {
                pthread_join(tid[i++],NULL);
            }
            i = 0;
        }
    }
  return 0;
}