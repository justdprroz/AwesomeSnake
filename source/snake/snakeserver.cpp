#include "snake/snakeserver.hpp"

int16_t SnakeGameServer::addSnake(){
    if (snakesAmount == maxSnakesAmount) return -1;
	snakesAmount++;
	snakes[snakesAmount - 1] = new Snake(snakesAmount, 20, 20);
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

		std::pair<float, float> lvec = {s->pos.first - s->lpos.first, s->pos.second - s->lpos.second};

		s->lpos = s->pos;
		switch (s->dir) {
			case LEFT:
				if (s->dir != RIGHT)
					s->pos.first-=s->step;
				break;
			case RIGHT:
				if (s->dir != LEFT)
					s->pos.first+=s->step;
				break;
			case UP:
				if (s->dir != DOWN)
					s->pos.second-=s->step;
				break;
			case DOWN:
				if (s->dir != UP)
					s->pos.second+=s->step;
				break;
			default:
				break;
		}

		std::pair<float, float> vec = {s->pos.first - s->lpos.first, s->pos.second - s->lpos.second};

		if (vec.first != lvec.first && vec.second != lvec.second){
			SnakePart* tmp = new SnakePart[s->lenght + 1];
			for(int i = 0; i < s->lenght; i++){
				tmp[i] = s->parts[i];
			}
			tmp[s->lenght] = SnakePart{{s->pos.first, s->pos.second}};
			delete[] s->parts;
			s->parts = tmp;
			s->lenght++;
		}
		
		s->parts[s->lenght - 1].head.first += vec.first;
		s->parts[s->lenght - 1].head.second += vec.second;

		int toDelete = 0;
		float path = std::sqrt(vec.first * vec.first + vec.second * vec.second);
		float backLenght = std::sqrt(std::pow(s->parts[toDelete].head.first - s->parts[toDelete].back.first, 2) + std::pow(s->parts[toDelete].head.second - s->parts[toDelete].back.second, 2));
		
		while(path > backLenght){
			path -= backLenght;
			toDelete++;
			backLenght = std::sqrt(std::pow(s->parts[toDelete].head.first - s->parts[toDelete].back.first, 2) + std::pow(s->parts[toDelete].head.second - s->parts[toDelete].back.second, 2));
		}
		SnakePart* tmp = new SnakePart[s->lenght - toDelete];
		for(int i = 0; i < s->lenght - toDelete; i++){
			tmp[i] = s->parts[i + toDelete];
		}
		delete[] s->parts;
		s->parts = tmp;
		s->lenght -= toDelete;
		backLenght = std::sqrt(std::pow(s->parts[0].head.first - s->parts[0].back.first, 2) + std::pow(s->parts[0].head.second - s->parts[0].back.second, 2));
		if (s->dir != STOP){
			float ratio = path / backLenght;
			s->parts[0].back.first += ((s->parts[0].head.first - s->parts[0].back.first) * ratio);
			s->parts[0].back.second += ((s->parts[0].head.second - s->parts[0].back.second) * ratio);
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
			// write(socket, &s->parts[i], sizeof(s->parts[i]));
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
	if (s->dir != STOP){
		std::pair<float, float> vec = {s->pos.first - s->lpos.first, s->pos.second - s->lpos.second};
		s->parts[0].back.first += ((s->parts[0].head.first - s->parts[0].back.first) * -1);
		s->parts[0].back.second += ((s->parts[0].head.second - s->parts[0].back.second) * -1);
	}
}