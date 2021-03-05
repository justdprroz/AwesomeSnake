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
			dx = s->pos.x - fruits[i].x;
			dy = s->pos.y - fruits[i].y;
			int range = 1;
			if( -range <= dx && dx <= range && -range <= dy && dy <= range){
				fruits[i].x = getRandomReal<float>(0, width - 1);
				fruits[i].y = getRandomReal<float>(0, height - 1);
				addPart(s);
			}
		}

		vec2f lvec = {s->pos.x - s->lpos.x, s->pos.y - s->lpos.y};

		s->lpos = s->pos;
		
		s->sdir = s->dir;

		switch (s->dir) {
			case LEFT:
				s->pos.x-=s->step;
				break;
			case RIGHT:
				s->pos.x+=s->step;
				break;
			case UP:
				s->pos.y-=s->step;
				break;
			case DOWN:
				s->pos.y+=s->step;
				break;
			default:
				break;
		}

		vec2f vec = {s->pos.x - s->lpos.x, s->pos.y - s->lpos.y};

		if (vec != lvec){
			SnakePart* tmp = new SnakePart[s->lenght + 1];
			for(int i = 0; i < s->lenght; i++){
				tmp[i] = s->parts[i];
			}
			tmp[s->lenght] = SnakePart{s->lpos, s->lpos, {float(sgn(vec.x)), float(sgn(vec.y))}, 
										{float(-sgn(vec.x)), float(-sgn(vec.y))}};
			delete[] s->parts;
			s->parts = tmp;
			s->lenght++;
		}

		if(s->parts[s->lenght - 1].vecHead == vec2f{0, 0}) {
			s->parts[s->lenght - 1].vecHead = vec2f({float(sgn(vec.x)), float(sgn(vec.y))});
		}
		
		if(s->parts[s->lenght - 1].vecBack == vec2f{0, 0}) {
			s->parts[s->lenght - 1].vecBack = vec2f({float(-sgn(vec.x)), float(-sgn(vec.y))});
		}

		SnakePart* first = &s->parts[s->lenght - 1];

		first->head.x += first->vecHead.x * s->step;
		first->head.y += first->vecHead.y * s->step;

		int toDelete = 0;
		SnakePart* last = &s->parts[toDelete];

		float path = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
		float backLenght = std::sqrt(std::pow(last->head.x - last->back.x, 2) + 
										std::pow(last->head.y - last->back.y, 2));
		
		while(path > backLenght){
			path -= backLenght;
			toDelete++;
			last = &s->parts[toDelete];
			backLenght = std::sqrt(std::pow(last->head.x - last->back.x, 2) + 
										std::pow(last->head.y - last->back.y, 2));
		}

		SnakePart* tmp = new SnakePart[s->lenght - toDelete];
		for(int i = 0; i < s->lenght - toDelete; i++){
			tmp[i] = s->parts[i + toDelete];
		}
		delete[] s->parts;
		s->parts = tmp;
		s->lenght -= toDelete;
		
		last = &s->parts[0];

		backLenght = std::sqrt(std::pow(last->head.x - last->back.x, 2) + 
								std::pow(last->head.y - last->back.y, 2));
		if (s->dir != STOP) {
			float ratio = path / backLenght;
			last->back += (last->head - last->back) * ratio;
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
		write(socket, &s->sdir, sizeof(s->sdir));
		write(socket, &s->id, sizeof(s->id));
		write(socket, &s->pos, sizeof(s->pos));
		write(socket, &s->lenght, sizeof(s->lenght));
		for(int i = 0; i < s->lenght; i++) {
			write(socket, &s->parts[i].head, sizeof(s->parts[i].head));
			write(socket, &s->parts[i].back, sizeof(s->parts[i].back));
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
	s->parts[0].back += s->parts[0].vecBack * s->step;
}