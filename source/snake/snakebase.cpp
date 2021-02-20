#include "snake/snakebase.hpp"

Snake::Snake(uint16_t i, uint16_t w, uint16_t h){
    id = i;
    pos = {getRandom<double>(0, w), getRandom<double>(0, h)};
    alive = true;
    dir = STOP;
    lenght = 1;
    step = 1.1;
    lpos = pos;
    parts = new std::pair<double, double>[1];
    parts[0] = {lpos.first, lpos.second};
}

Snake::Snake(uint16_t i, double x, double y){
    id = i;
    pos = {x, y};
    parts = new std::pair<double, double>(pos.first, pos.second);
    alive = true;
    dir = STOP;
    lenght = 1;
    step = 1.1;
    lpos = pos;
    parts = new std::pair<double, double>[1];
    parts[0] = {lpos.first, lpos.second};
}

SnakeGameBase::SnakeGameBase(uint16_t msa, uint16_t fa){
    SnakeGameBase(64, 64, msa, fa);
}

SnakeGameBase::SnakeGameBase(uint16_t w, uint16_t h, uint16_t msa, uint16_t fa){
    width = w;
	height = h;
	snakesAmount = 0;
    maxSnakesAmount = msa;
	fruitsAmount = fa;
	snakes = new Snake*[maxSnakesAmount];
	fruits = new std::pair<double, double>[fruitsAmount];
    for(int i = 0; i < fruitsAmount; i++){
        fruits[i] = {getRandom<double>(0, w), getRandom<double>(0, h)};
    }
}

uint16_t SnakeGameBase::getSnakeId(Snake* s){
    return s->id;
}

uint16_t SnakeGameBase::getSnakeIndexById(uint16_t id){
    for(int i = 0; i < snakesAmount; i++){
		if(getSnakeId(snakes[i]) == id)
			return i;
	}
	return -1;
};