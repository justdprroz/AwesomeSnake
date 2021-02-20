#include "snake/snakeclient.hpp"

void SnakeGameClient::draw(sf::RenderWindow* w, int as){
    std::cout << as << '\n';
	for(int i = 0; i < fruitsAmount; i++){
		sf::RectangleShape cell;
		cell.setSize(sf::Vector2f(20, 20));
		cell.setFillColor(sf::Color::Red);
		cell.setPosition(fruits[i].first * 20, fruits[i].second * 20);
		w->draw(cell);
	}
	for(int i = 0; i < snakesAmount; i++){
        Snake* s = snakes[i];
        sf::RectangleShape cell;
        for (int i = s->lenght - 1; i >= 0; i--){
            cell.setSize(sf::Vector2f(20, 20));
            int R = i * (255 / (s->lenght - 1 + (s->lenght == 1))), G = 255, B = 0;
            if(as == s->id){
                cell.setFillColor(sf::Color(R, G, B, 255));
            } else {
                int Y = 255 - (R + G + B) / 3;
                cell.setFillColor(sf::Color(Y, Y, Y));
            }
            cell.setPosition(s->parts[i].first * 20, s->parts[i].second * 20);
            w->draw(cell);
        }
    };
}

void SnakeGameClient::get(int socket){
    int ret;
	ret = read(socket, &width, sizeof(width));
	if( (ret == -1 || ret != sizeof(width))) {
		std::cout << "Error readind SnakeGame.W\n";
	}
	ret = read(socket, &height, sizeof(height));
	if( (ret == -1 || ret != sizeof(height))) {
		std::cout << "Error readind SnakeGame.H\n";
	}
	ret = read(socket, &snakesAmount, sizeof(snakesAmount));
	if( (ret == -1 || ret != sizeof(snakesAmount))) {
		std::cout << "Error readind SnakeGame.SnakesAmount\n";
	}
	ret = read(socket, &fruitsAmount, sizeof(fruitsAmount));
	if( (ret == -1 || ret != sizeof(fruitsAmount))) {
		std::cout << "Error readind SnakeGame.FruitsAmount\n";
	}
	for(int i = 0; i < fruitsAmount; i++){
		ret = read(socket, &fruits[i], sizeof(fruits[i]));
		if( (ret == -1 || ret != sizeof(fruits[i]))) {
			std::cout << "Error readind SnakeGame.FRUITSARR[" << i << "]\n";
		}
	}
	for(int i = 0; i < snakesAmount; i++){
        snakes[i] = new Snake(i, (uint16_t)50, (uint16_t)50);
        Snake* s = snakes[i];
        int ret;
        ret = read(socket, &s->dir, sizeof(s->dir));
        if( (ret == -1 || ret != sizeof(s->dir))) {
            std::cout << "Error readind DIR\n";
        }
        ret = read(socket, &s->id, sizeof(s->id));
        if( (ret == -1 || ret != sizeof(s->id))) {
            std::cout << "Error readind ID\n";
        }
        ret = read(socket, &s->pos, sizeof(s->pos));
        if( (ret == -1 || ret != sizeof(s->pos))) {
            std::cout << "Error readind POS\n";
        }
        ret = read(socket, &s->lenght, sizeof(s->lenght));
        if( (ret == -1 || ret != sizeof(s->lenght))) {
            std::cout << "Error readind LEN\n";
        }
        delete s->parts;
	    s->parts = new std::pair<double, double>[s->lenght];
	    for(int i = 0; i < s->lenght; i++){
            int ret;
            ret = read(socket, &s->parts[i], sizeof(s->parts[i]));
            if( (ret == -1 || ret != sizeof(s->parts[i]))) {
                std::cout << "Error readind PART " << i << '\n';
            }
	    }
	}
}

void SnakeGameClient::sendSnakeDir(int socket, int id){
	direction tmp;
	tmp = snakes[getSnakeIndexById(id)]->dir;
    write(socket, &tmp, sizeof(tmp));
}

void SnakeGameClient::handleEvents(sf::Event e, uint16_t id){
    Snake* s = snakes[getSnakeIndexById(id)];
    if(e.type == sf::Event::KeyPressed){
		if (e.key.code == sf::Keyboard::W){
			s->dir = UP;
		}
		if (e.key.code == sf::Keyboard::D){
			s->dir = RIGHT;
		}
		if (e.key.code == sf::Keyboard::S){
			s->dir = DOWN;
		}
		if (e.key.code == sf::Keyboard::A){
			s->dir = LEFT;
		}
	}
}