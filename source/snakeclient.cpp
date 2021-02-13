#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

#include <thread>

#include <chrono>
#include "snake.hpp"
// #define IP "192.168.1.50"
// #define PORT 51090
SnakeGame game;
int id;

void commsock(int sock){
	while(true) {
		game.sendSnakeDir(sock, id);
		game.get(sock);
	}
}

int logic(){
	while (true)
	{
		game.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

int main(int argc, char const *argv[]) 
{ 
	int sock = 0; 
	struct sockaddr_in serv_addr;  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	write(sock, new int(100), sizeof(100));
	read(sock, &id, sizeof(id));
	int Width = 1000, Height = 1000;
    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML works!");
	sf::View view;
	view.setSize(sf::Vector2f(Width, Height));
	game.get(sock);
	window.setView(view);
	std::thread(commsock, sock).detach();
	// std::thread(logic).detach();
	while (window.isOpen())
    {	
		Snake* s = game.getSnakePtr(id);
        sf::Event event;
        while (window.pollEvent(event))
        {	
			s->handleEvents(event);
            if (event.type == sf::Event::Closed){
				int s = -100;
				write(sock, &s, sizeof(s));
				write(sock, &id, sizeof(id));
                window.close();
				return 0;
			}
			if (event.type == sf::Event::Resized) {
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
        }
		game.draw(&window, id);
    }
	return 0;
} 
