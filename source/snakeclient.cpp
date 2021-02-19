#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "snake.hpp"

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
	int sock = 0;
	int signal = 100;
	struct sockaddr_in serv_addr;  
	sock = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &serv_addr.sin_addr);
	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	int id;
	int ret;
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
		Snake* s = game.getSnakePtr(id);
        sf::Event event;
        while (window.pollEvent(event))
        {	
			s->handleEvents(event);
            if (event.type == sf::Event::Closed){
                window.close();
				return 0;
			}
			if (event.type == sf::Event::Resized) {
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
        }
		game.sendSnakeDir(sock, s->getId());
		game.get(sock);
		game.draw(&window, s->getId());
    }
	return 0;
} 