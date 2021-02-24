// #include "snake/snake.hpp"
#include "snake/snakeclient.hpp"

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
	std::cout << id << '\n';
	int Width = 1000, Height = 1000;
    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML works!");
	sf::View view;
	view.setSize(sf::Vector2f(Width, Height));
	SnakeGameClient game(100, 100, 10, 10);
	game.get(sock);
	window.setView(view);
	while (window.isOpen())
    {	
		int sig;
		sf::Event event;
        while (window.pollEvent(event))
        {	
			game.handleEvents(event, id);
            if (event.type == sf::Event::Closed){
                window.close();
				sig = 100;
				write(sock, &sig, sizeof(sig));
				write(sock, &id, sizeof(id));
				return 0;
			}
			if (event.type == sf::Event::Resized) {
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
        }
		window.clear();
		sig = 0;
		write(sock, &sig, sizeof(sig));
		game.sendSnakeDir(sock, id);
		game.get(sock);
		game.draw(&window, id);
		window.display();
    }
	return 0;
} 