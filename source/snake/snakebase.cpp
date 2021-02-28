#include "snake/snakebase.hpp"

SnakePart::SnakePart(vec2f h, vec2f b, vec2f vh, vec2f vb){
    head = h;
    back = b;
    vecHead = vh;
    vecBack = vb;
}

Snake::Snake(){
    alive = true;
    dir = STOP;
    lenght = 1;
    parts = new SnakePart[1];
};

Snake::Snake(int16_t i, float s, float g, vec2f p) : Snake(){
    id = i;
    pos = p;
    lpos = pos;
    step = s;
    growth = g;
    parts[0] = SnakePart(p, p, {0, 0}, {0, 0});
}

Snake::Snake(int16_t i, float x, float y) : Snake(i, 1, 1, vec2f(x, y)) {}

Snake::Snake(int16_t i, vec2f p) : Snake(i, 1, 1, p){}

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
	fruits = new vec2f[fruitsAmount];
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