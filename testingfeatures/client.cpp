#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#define IP "127.0.0.1"
#define PORT 8080
int main(int argc, char const *argv[]) 
{ 
	int sock = 0; 
	struct sockaddr_in serv_addr;  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);	
	inet_pton(AF_INET, IP, &serv_addr.sin_addr);
	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    int myint = atoi(argv[1]);
    for(int i = 0; i < 1000; i++){
        myint++;
        write(sock, &myint, sizeof(myint));
        read(sock, &myint, sizeof(myint));
	    std::cout << myint << '\n';
    }
	return 0;
} 
