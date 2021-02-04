#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <iostream>
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket;
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( 8080 );
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);    
    while(1){        
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        int myint;
        for(int i = 0; i < 1000; i++){  
            read(new_socket, &myint, sizeof(myint));
            myint++;
            write(new_socket, &myint, sizeof(myint));
            std::cout << myint << '\n';
        }
    }
	return 0; 
}