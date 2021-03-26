#include "snake/snakeclient.hpp"

void SnakeGameClient::draw(sf::RenderWindow* w, int as){
	int tileSize = 10;
	for(int i = 0; i < fruitsAmount; i++){
		sf::RectangleShape cell;
		cell.setSize(sf::Vector2f(tileSize, tileSize));
		cell.setFillColor(sf::Color::Red);
		cell.setPosition(fruits[i].x * tileSize - tileSize / 2, fruits[i].y * tileSize - tileSize / 2);
		w->draw(cell);
	}
	for(int i = 0; i < snakesAmount; i++){
        Snake* s = snakes[i];
        sf::RectangleShape cell;
        for (int i = 0; i < s->lenght; i++){
            cell.setSize(sf::Vector2f(std::abs(s->parts[i].head.x - s->parts[i].back.x) * tileSize + tileSize, 
							std::abs(s->parts[i].head.y - s->parts[i].back.y) * tileSize + tileSize));
            int R = i * (255. / (s->lenght - 1 + (s->lenght == 1))), G = 255, B = 0;
            if(as == s->id){
                cell.setFillColor(sf::Color(R, G, B, 255));
            } else {
                int Y = 255 - (R + G + B) / 3;
                cell.setFillColor(sf::Color(Y, Y, Y));
            }
			cell.setPosition(std::min(s->parts[i].head.x, s->parts[i].back.x) * tileSize - tileSize / 2, 
								std::min(s->parts[i].head.y, s->parts[i].back.y) * tileSize - tileSize / 2);
            w->draw(cell);
        }
		cell.setSize({tileSize, tileSize});
		cell.setPosition({s->pos.x * tileSize - tileSize / 2, s->pos.y * tileSize - tileSize / 2});
		cell.setFillColor(sf::Color::Blue);
		w->draw(cell);
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
        snakes[i] = new Snake(i, (int16_t)50, (int16_t)50);
        Snake* s = snakes[i];
        int ret;
        ret = read(socket, &s->dir, sizeof(s->dir));
        if( (ret == -1 || ret != sizeof(s->dir))) {
            std::cout << "Error readind DIR\n";
        }
		ret = read(socket, &s->sdir, sizeof(s->sdir));
        if( (ret == -1 || ret != sizeof(s->sdir))) {
            std::cout << "Error readind sDIR\n";
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
	    s->parts = new SnakePart[s->lenght];
	    for(int i = 0; i < s->lenght; i++){
            int ret;
			ret = read(socket, &s->parts[i].head, sizeof(s->parts[i].head));
			ret = read(socket, &s->parts[i].back, sizeof(s->parts[i].back));
            if( (ret == -1 || ret != sizeof(s->parts[i].head))) {
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

void SnakeGameClient::handleEvents(sf::Event e, int16_t id){
    Snake* s = snakes[getSnakeIndexById(id)];
    if(e.type == sf::Event::KeyPressed){
		if (e.key.code == sf::Keyboard::W){
			if (s->sdir != DOWN)
				s->dir = UP;
		}
		if (e.key.code == sf::Keyboard::D){
			if (s->sdir != LEFT)
				s->dir = RIGHT;
		}
		if (e.key.code == sf::Keyboard::S){
			if (s->sdir != UP)
				s->dir = DOWN;
		}
		if (e.key.code == sf::Keyboard::A){
			if (s->sdir != RIGHT)

				s->dir = LEFT;
		}
	}
}