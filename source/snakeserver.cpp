#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "snake.hpp"
// #define IP "192.168.1.50"
// #define PORT 51090

int serverSocket, newSocket, opt = 1;
sockaddr_in address;
sockaddr_storage serverStorage;
socklen_t addr_size;
SnakeGame game;

void commsock(int arg){
    int newSocket = arg;
    int sig = 0;
    while (true)
    {
        int ret;
        ret = read(newSocket, &sig, sizeof(sig));
        if( (ret == -1 || ret != sizeof(sig))) {
		    std::cout << "Error readind signal\n";
	    }
        if(sig == 0){
            game.send(newSocket);
        }
        if(sig == 3){
            game.getSnakeDir(newSocket);
        }
        if(sig == 100){
            int id = game.addSnake();
            write(newSocket, &id, sizeof(id));
        }
        if(sig == -100){
            int id;
            read(newSocket, &id, sizeof(id));
            game.removeSnake(id);
            close(newSocket);
            return;
        }
        sig = -1000;
    }
}

void logic(){
    while(true){
        game.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int argc, char const *argv[]) 
{   
    int port = 22222;
	const char* ip = "127.0.0.1";
	if (argc > 1) {
		ip = argv[1];
	}
	if (argc > 2){
		port = std::stoi(argv[2]);
	}
    srand(time(NULL));
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	address.sin_family = AF_INET; 
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(ip);

    bind(serverSocket, (struct sockaddr *)&address, sizeof(address));
    listen(serverSocket, 50);

    std::thread log(logic);
    while(1) {
        addr_size = sizeof(serverStorage);
        newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, (socklen_t*)&addr_size);
        std::thread(commsock, newSocket).detach();
    }
	return 0;
}