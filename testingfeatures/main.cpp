#include "snake.hpp"
#include <thread>
#include <chrono>
int main()
{
	srand(time(NULL));
	int Width = 1000, Height = 1000;
    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML works!");
	sf::View view;
	view.setSize(sf::Vector2f(Width, Height));
	window.setView(view);
	SnakeGame game;
	int si = game.addSnake();
	for(int i = 0; i < 1; i++)
		game.addSnake();
    while (window.isOpen())
    {
		Snake* s = game.getsnakeptr(si);
        sf::Event event;
        while (window.pollEvent(event))
        {	
			s->handleEvents(event);
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::Resized) {
				sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
        }	
		game.Step();
		game.Draw(&window, s->getId());
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}