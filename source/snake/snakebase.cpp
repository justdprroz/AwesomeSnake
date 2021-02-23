#include "snake/snakebase.hpp"

SnakePart::SnakePart(std::pair<float, float> h, std::pair<float, float> b){
    head = h;
    back = b;
}

SnakePart::SnakePart(std::pair<float, float> p){
    head = p;
    back = p;
}

// SnakePart::SnakePart(std::pair<float, float> l, std::pair<float, float> r, std::pair<float, float> v) {
//     lu = l;
//     rd = r;
//     ve = v;
// }

Snake::Snake(int16_t i, float x, float y){
    id = i;
    pos = {x, y};
    alive = true;
    dir = STOP;
    lenght = 1;
    step = 1;
    lpos = pos;
    parts = new SnakePart[1];
    parts[0] = SnakePart({x, y}, {x + 1, y + 1});
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