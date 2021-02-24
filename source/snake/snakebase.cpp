#include "snake/snakebase.hpp"

Snake::Snake(int16_t i, float x, float y){
    id = i;
    pos = {x, y};
    parts = new std::pair<float, float>(pos.first, pos.second);
    alive = true;
    dir = STOP;
    lenght = 1;
    step = 1;
    lpos = pos;
    parts = new std::pair<float, float>[1];
    parts[0] = {lpos.first, lpos.second};
}

SnakeGameBase::SnakeGameBase(int16_t msa, int16_t fa){
    SnakeGameBase(64, 64, msa, fa);
}

SnakeGameBase::SnakeGameBase(int16_t w, int16_t h, int16_t msa, int16_t fa){
    width = w;
	height = h;
	snakesAmount = 0;
    maxSnakesAmount = msa;
	fruitsAmount = fa;
	snakes = new Snake*[maxSnakesAmount];
	fruits = new std::pair<float, float>[fruitsAmount];
    for(int i = 0; i < fruitsAmount; i++){
        fruits[i] = {getRandomReal<float>(0, w - 1), getRandomReal<float>(0, h - 1)};
    }
}

int16_t SnakeGameBase::getSnakeId(Snake* s){
    return s->id;
}

int16_t SnakeGameBase::getSnakeIndexById(int16_t id){
    for(int i = 0; i < snakesAmount; i++){
		if(getSnakeId(snakes[i]) == id)
			return i;
	}
	return -1;
};