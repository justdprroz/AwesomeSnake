#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "snake.hpp"
#define IP "192.168.1.40"
#define PORT 21090

int main(int argc, char const *argv[]) 
{ 
	int sock = 0; 
	struct sockaddr_in serv_addr;  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);	
	inet_pton(AF_INET, IP, &serv_addr.sin_addr);
	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	write(sock, new int(100), sizeof(100));
	int id;
	read(sock, &id, sizeof(id));
	int Width = 1000, Height = 1000;
    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML works!");
	sf::View view;
	view.setSize(sf::Vector2f(Width, Height));
	SnakeGame game;
	game.get(sock);
	window.setView(view);
	while (window.isOpen())
    {	
		Snake* s = game.getsnakeptr(id);
        sf::Event event;
        while (window.pollEvent(event))
        {	
			s->handleEvents(event);
            if (event.type == sf::Event::Closed){
				int s = -100;
				write(sock, &s, sizeof(s));
                window.close();
				return 0;
			}
			if (event.type == sf::Event::Resized) {
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
        }
		Directon d;
		d = game.SNAKES[game.getIndex(s->getId())]->DIR;
		game.sendSnakeDir(sock, d, s->getId());
		// game.getStatic(sock);
		// game.getDynamic(sock);
		game.get(sock);
		game.Draw(&window, s->getId());
    }
	return 0;
} 
