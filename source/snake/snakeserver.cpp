#include "snake/snakeserver.hpp"

uint16_t SnakeGameServer::addSnake(){
    if (snakesAmount == maxSnakesAmount) return -1;
	snakesAmount++;
	snakes[snakesAmount - 1] = new Snake(snakesAmount, width, height);
	std::cout << snakes[snakesAmount - 1]->id << '\n';
	return getSnakeId(snakes[snakesAmount - 1]);
}

void SnakeGameServer::removeSnake(uint16_t id){
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
			double dx, dy;
			dx = s->pos.first - fruits[i].first;
			dy = s->pos.second - fruits[i].second;
			if( -1 <= dx && dx <= 1 && -1 <= dy && dy <= 1){
				fruits[i].first = getRandom<double>(0, width);
				fruits[i].second = getRandom<double>(0, height);
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
		auto transfer = [] (double a, uint16_t b) {
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
		std::pair<double,double> prev2;
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

void SnakeGameServer::getSnakeDirection(int socket, uint16_t id){
	int ret;
	direction tmp;
	ret = read(socket, &tmp, sizeof(tmp));
	if( (ret == -1 || ret != sizeof(tmp))) {
		std::cout << "Error getting direction of snake " << id << '\n';
		return;
	}
	snakes[getSnakeIndexById(id)]->dir = tmp;
}

void SnakeGameServer::addPart(uint16_t id){
	Snake * s = snakes[getSnakeIndexById(id)];
	addPart(s);
}

void SnakeGameServer::addPart(Snake* s){
	s->lenght++;
	std::pair<double,double>* tmp = new std::pair<double,double>[s->lenght];
	for(int i = 0; i < s->lenght - 1; i++){
		tmp[i] = s->parts[i];
	}
	tmp[s->lenght - 1] = {s->lpos.first, s->lpos.second};
	delete[] s->parts;
	s->parts = tmp;
}