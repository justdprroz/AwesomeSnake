#include "snake.hpp"

SnakeGame::SnakeGame(){
	W = 50;
	H = 50;
	SnakesAmount = 0;
	FruitsAmount = 0;
	FRUITSARR = new std::pair<int, int>[1];
	SNAKES = new Snake*;
}

int SnakeGame::addSnake(){
	SnakesAmount++;
	if(FruitsAmount < SnakesAmount)
		addFruit();
	Snake** tmp = new Snake*[SnakesAmount];
	for(int i = 0; i < SnakesAmount; i++){
		tmp[i] = SNAKES[i];
	}
	tmp[SnakesAmount - 1] = new Snake(this, SnakesAmount);
	delete[] SNAKES;
	SNAKES = tmp;
	return SNAKES[SnakesAmount - 1]->getId();
}

void SnakeGame::removeSnake(int id){
	while(LOCK) {
	}
	LOCK = true;
	SnakesAmount--;
	Snake** tmp = new Snake*[SnakesAmount];
	int o = 0;
	for(int i = 0; i < SnakesAmount + 1; i++){
		if(SNAKES[i]->getId() == id){
			o = 1;
			continue;
		}
		tmp[i - o] = SNAKES[i];
	}
	delete[] SNAKES;
	SNAKES = tmp;
	LOCK = false;
}

void SnakeGame::addFruit(){
	FruitsAmount++;
	std::pair<int, int>* tmp = new std::pair<int, int>[FruitsAmount];
	for(int i = 0; i < FruitsAmount - 1; i++){
		tmp[i] = FRUITSARR[i];
	}
	tmp[FruitsAmount - 1] = {rand() % W, rand() % H};
	delete[] FRUITSARR;
	FRUITSARR = tmp;
}

int SnakeGame::tryEat(int x, int y){
	for(int i = 0; i < FruitsAmount; i++){
		if(FRUITSARR[i] == std::make_pair(x, y)){
			for(int i = 0; i < FruitsAmount + 1; i++){
				if(FRUITSARR[i] == std::make_pair(x, y)){
					FRUITSARR[i] = {rand() % W, rand() % H};
				}
			}
			return 1;
		}
	}
	return 0;
}
Snake* SnakeGame::getSnakePtr(int id){
	for(int i = 0; i < SnakesAmount; i++){
		if (SNAKES[i]->getId() == id){
			return SNAKES[i];
		}
	}
	return nullptr;
}

void SnakeGame::get(int s){
	int sig = 0;
	write(s, &sig, sizeof(0));
	read(s, &W, sizeof(W));
	read(s, &H, sizeof(H));
	read(s, &SnakesAmount, sizeof(SnakesAmount));
	read(s, &FruitsAmount, sizeof(FruitsAmount));
	delete[] FRUITSARR;
	FRUITSARR = new std::pair<int, int>[FruitsAmount];
	for(int i = 0; i < FruitsAmount; i++){
		read(s, &FRUITSARR[i], sizeof(FRUITSARR[i]));
	}
	delete[] SNAKES;
	SNAKES = new Snake*[SnakesAmount];
	for(int i = 0; i < SnakesAmount; i++){
		SNAKES[i] = new Snake(this, i);
		SNAKES[i]->getStatic(s);
		SNAKES[i]->getDynamic(s);
	}
}

void SnakeGame::send(int s){
	while(LOCK) {

	}
	LOCK = true;
	int tfa = FruitsAmount, tsa = SnakesAmount;
	write(s, &W, sizeof(W));
	write(s, &H, sizeof(H));
	write(s, &SnakesAmount, sizeof(SnakesAmount));
	write(s, &FruitsAmount, sizeof(FruitsAmount));
	for(int i = 0; i < tfa; i++){
		write(s, &FRUITSARR[i], sizeof(FRUITSARR[i]));
	}
	for(int i = 0; i < tsa; i++){
		SNAKES[i]->sendStatic(s);
		SNAKES[i]->sendDynamic(s);
	}
	LOCK = false;
}

void SnakeGame::draw(sf::RenderWindow* w, int msnakeid){
	w->clear(sf::Color::White);
	for(int i = 0; i < FruitsAmount; i++){
		sf::RectangleShape cell;
		cell.setSize(sf::Vector2f(20, 20));
		cell.setFillColor(sf::Color::Red);
		cell.setPosition(FRUITSARR[i].first * 20, FRUITSARR[i].second * 20);
		w->draw(cell);
	}

	for(int i = 0; i < SnakesAmount; i++)
		SNAKES[i]->draw(w, SNAKES[i]->getId()==msnakeid);
	w->display();
}

int SnakeGame::getIndex(int i){
	for(int j = 0; j < SnakesAmount; j++){
		if(SNAKES[j]->getId() == i)
			return j;
	}
	return -1;
}

void SnakeGame::sendSnakeDir(int sock, int id){
	int sig = 3;
	write(sock, &sig, sizeof(3));
	write(sock, &id, sizeof(id));
	DIRECTION t;
	t = SNAKES[getIndex(id)]->getDir();
	write(sock, &t, sizeof(t));
}

void SnakeGame::getSnakeDir(int sock){
	int id;
	read(sock, &id, sizeof(id));
	DIRECTION t;
	read(sock, &t, sizeof(DIRECTION));
	SNAKES[getIndex(id)]->setDir(t);
}


void SnakeGame::step(){
	while(LOCK) {
	}
	LOCK = true;
	for(int i = 0; i < SnakesAmount; i++)
		SNAKES[i]->update();
	LOCK = false;
}

std::pair<int, int> SnakeGame::getSize(){
	return {W, H};
}

void Snake::getStatic(int s){
	read(s, &DIR, sizeof(DIR));
	read(s, &ID, sizeof(ID));
	read(s, &POS, sizeof(POS));
	read(s, &LEN, sizeof(LEN));
}

void Snake::getDynamic(int s){
	delete[] PARTS;
	PARTS = new std::pair<int, int>[LEN];
	for(int i = 0; i < LEN; i++){
		read(s, &PARTS[i], sizeof(PARTS[i]));
	}
}

void Snake::sendStatic(int s){
	write(s, &DIR, sizeof(DIR));
	write(s, &ID, sizeof(ID));
	write(s, &POS, sizeof(POS));
	write(s, &LEN, sizeof(LEN));
}

void Snake::sendDynamic(int s){
	for(int i = 0; i < LEN; i++){
		write(s, &PARTS[i], sizeof(PARTS[i]));
	}	
}

Snake::Snake(SnakeGame* ses_ptr, int id){
	LEN = 0;
	SESSION = ses_ptr;
	POS = {rand() % SESSION->getSize().first, rand() % SESSION->getSize().second};
	LPOS=POS;
	PARTS = new std::pair<int,int>[1];
	ID = id;
	addPart();
}

int Snake::getId(){
	return ID;
}

void Snake::handleEvents(sf::Event e){
	if(e.type == sf::Event::KeyPressed){
		if (e.key.code == sf::Keyboard::W){
			DIR = U;
		}
		if (e.key.code == sf::Keyboard::D){
			DIR = R;
		}
		if (e.key.code == sf::Keyboard::S){
			DIR = D;
		}
		if (e.key.code == sf::Keyboard::A){
			DIR = L;
		}
	}
}

void Snake::setDir(DIRECTION d){
	DIR = d;
}

DIRECTION Snake::getDir(){
	return DIR;
}

void Snake::update(){
	if(SESSION->tryEat(POS.first, POS.second)){
		addPart();
	}
	LPOS = POS;
	switch (DIR)
	{
	case L:
		POS.first--;
		break;
	case R:
		POS.first++;
		break;
	case U:
		POS.second--;
		break;
	case D:
		POS.second++;
		break;
	default:
		break;
	}
	auto div = [] (int a, int b) {
        return (b + (a%b)) % b;
    };
	POS.first=div(POS.first, SESSION->getSize().first);
	POS.second=div(POS.second, SESSION->getSize().second);
	if (LEN > 0){
		std::pair<int,int> prev2;
		for (int i = 0; i < LEN; i++)
		{
			prev2 = PARTS[i];
			PARTS[i] = LPOS;
			LPOS = prev2;
		}
	}
}

void Snake::draw(sf::RenderWindow* w, bool active){
	sf::RectangleShape cell;
	for (int i = LEN - 1; i >= 0; i--){
		cell.setSize(sf::Vector2f(20, 20));
		int R = i * (255 / (LEN - 1 + (LEN == 1))), G = 255, B = 0;
		if(active){
			cell.setFillColor(sf::Color(R, G, B, 255));
		} else {
			int Y = (R + G + B) / 3;
			cell.setFillColor(sf::Color(255 - Y));
		}
		cell.setPosition(PARTS[i].first * 20, PARTS[i].second * 20);
		w->draw(cell);
	}
}

void Snake::addPart(){
	LEN++;
	std::pair<int,int>* tmp = new std::pair<int,int>[LEN];
	for(int i = 0; i < LEN - 1; i++){
		tmp[i] = PARTS[i];
	}
	tmp[LEN - 1] = {LPOS.first, LPOS.second};
	delete[] PARTS;
	PARTS = tmp;
}