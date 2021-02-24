#include "snake/snakeserver.hpp"

int16_t SnakeGameServer::addSnake(){
    if (snakesAmount == maxSnakesAmount) return -1;
	snakesAmount++;
	snakes[snakesAmount - 1] = new Snake(snakesAmount, getRandomReal<float>(0, width - 1), getRandomReal<float>(0, height - 1));
	return getSnakeId(snakes[snakesAmount - 1]);
}

void SnakeGameServer::removeSnake(int16_t id){
	if(snakesAmount == 0) return;
	snakesAmount--;
	int o = 0;
	for(int i = 0; i < snakesAmount; i++){
		if(getSnakeId(snakes[i]) == id){
			o = 1;
		}
		snakes[i] = snakes[i + o];
	}
}

void SnakeGameServer::step(){
    for(int i = 0; i < snakesAmount; i++){
		Snake* s = snakes[i];
		for(int i = 0; i <= fruitsAmount; i++){
			float dx, dy;
			dx = s->pos.first - fruits[i].first;
			dy = s->pos.second - fruits[i].second;
			int range = 2;
			if( -range <= dx && dx <= range && -range <= dy && dy <= range){
				fruits[i].first = getRandomReal<float>(0, width - 1);
				fruits[i].second = getRandomReal<float>(0, height - 1);
				addPart(s);
				break;
			}
		}
		s->lpos = s->pos;
		switch (s->dir) {
			case LEFT:
				s->pos.first-=s->step;
				break;
			case RIGHT:
				s->pos.first+=s->step;
				break;
			case UP:
				s->pos.second-=s->step;
				break;
			case DOWN:
				s->pos.second+=s->step;
				break;
			default:
				break;
		}
		auto transfer = [] (float a, int16_t b) {
			if (a >= b){
				a -= b;
			}
			if (a < 0){
				a += b;
			}
        	return a;
    	};
		s->pos.first=transfer(s->pos.first, width);
		s->pos.second=transfer(s->pos.second, height);
		std::pair<float,float> prev2;
		for (int i = 0; i < s->lenght; i++)
		{
			prev2 = s->parts[i];
			s->parts[i] = s->lpos;
			s->lpos = prev2;
		}
    }
}

void SnakeGameServer::send(int socket){
	write(socket, &width, sizeof(width));
	write(socket, &height, sizeof(height));
	write(socket, &snakesAmount, sizeof(snakesAmount));
	write(socket, &fruitsAmount, sizeof(fruitsAmount));
	for(int i = 0; i < fruitsAmount; i++){
		write(socket, &fruits[i], sizeof(fruits[i]));
	}
	for(int i = 0; i < snakesAmount; i++){
		Snake* s = snakes[i];
		write(socket, &s->dir, sizeof(s->dir));
		write(socket, &s->id, sizeof(s->id));
		write(socket, &s->pos, sizeof(s->pos));
		write(socket, &s->lenght, sizeof(s->lenght));
		for(int i = 0; i < s->lenght; i++) {
			write(socket, &s->parts[i], sizeof(s->parts[i]));
		}	
	}
}

void SnakeGameServer::getSnakeDirection(int socket, int16_t id){
	int ret;
	direction tmp;
	ret = read(socket, &tmp, sizeof(tmp));
	if( (ret == -1 || ret != sizeof(tmp))) {
		std::cout << "Error getting direction of snake " << id << '\n';
		return;
	}
	snakes[getSnakeIndexById(id)]->dir = tmp;
}

void SnakeGameServer::addPart(int16_t id){
	Snake * s = snakes[getSnakeIndexById(id)];
	addPart(s);
}

void SnakeGameServer::addPart(Snake* s){
	s->lenght++;
	std::pair<float,float>* tmp = new std::pair<float,float>[s->lenght];
	for(int i = 0; i < s->lenght - 1; i++){
		tmp[i] = s->parts[i];
	}
	tmp[s->lenght - 1] = {s->lpos.first, s->lpos.second};
	delete[] s->parts;
	s->parts = tmp;
}