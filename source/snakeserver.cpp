#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "snake.hpp"
#define IP "127.0.0.1"
#define PORT 21090
struct sockaddr_in address;
int server_fd, new_socket, opt = 1, addrlen = sizeof(address);

SnakeGame game;
void commsock(int sock);
void communicate(){
    while(1){
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        std::cout << new_socket << '\n';
        std::thread(commsock, new_socket).join();
    }
}

void commsock(int sock){
    int sig = 0;
    while (true)
    {
        read(new_socket, &sig, sizeof(sig));
        if(sig == 0){
            game.send(new_socket);
        }
        if(sig == 3){
            game.getSnakeDir(new_socket);
        }
        
        if(sig == 100){
            int id = game.addSnake();
            write(new_socket, &id, sizeof(id));
        }
        if(sig == -100){
            close(new_socket);
            std::terminate();
        }
        sig = -1000;
    }
    
}

void logic(){
    while(true){
        game.Step();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int argc, char const *argv[]) 
{   
    srand(time(NULL));
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    std::thread com(communicate);
    std::thread log(logic);
    com.join();
    log.join();
	return 0;
}