#include "snake.hpp"

SnakeGame::SnakeGame(){
	W = 50;
	H = 50;
	SnakesAmount = 0;
	FruitsAmount = 0;
	FRUITSARR = new std::pair<int, int>[1];
	SNAKES = new Snake*[1];
}

void SnakeGame::AddFruit(){
	FruitsAmount++;
	std::pair<int, int>* tmp = new std::pair<int, int>[FruitsAmount];
	for(int i = 0; i < FruitsAmount - 1; i++){
		tmp[i] = FRUITSARR[i];
	}
	tmp[FruitsAmount - 1] = {rand() % W, rand() % H};
	delete[] FRUITSARR;
	FRUITSARR = tmp;
}

Snake* SnakeGame::getsnakeptr(int id){
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
	// std::cout << "signal sent: " << 0 << '\n';

	read(s, &W, sizeof(W));
	// std::cout << "got W " << W << '\n';
	read(s, &H, sizeof(H));
	// std::cout << "got H " << H << '\n';
	read(s, &SnakesAmount, sizeof(SnakesAmount));
	// std::cout << "got SnakesAmount " << SnakesAmount << '\n';
	read(s, &FruitsAmount, sizeof(FruitsAmount));
	// std::cout << "got FruitsAmount " << FruitsAmount << '\n';
	delete[] FRUITSARR;
	FRUITSARR = new std::pair<int, int>[FruitsAmount];
	for(int i = 0; i < FruitsAmount; i++){
		read(s, &FRUITSARR[i], sizeof(FRUITSARR[i]));
		// std::cout << "got fruit coordinate of fruit " << i << ':' << FRUITSARR[i].first << ' ' << FRUITSARR[i].second << '\n';
	}
	delete[] SNAKES;
	SNAKES = new Snake*[SnakesAmount];
	for(int i = 0; i < SnakesAmount; i++){
		SNAKES[i] = new Snake(this, i);
		SNAKES[i]->getStatic(s);
		// std::cout << "got snake static " << i << '\n';
		SNAKES[i]->getDynamic(s);
		// std::cout << "got snake dynamic " << i << '\n';
		// std::cout << "got Snake " << i << '\n';
	}
}

void SnakeGame::send(int s){
	int tfa = FruitsAmount, tsa = SnakesAmount;
	write(s, &W, sizeof(W));
	// std::cout << "sent W " << W << '\n';
	write(s, &H, sizeof(H));
	// std::cout << "sent H " << H << '\n';
	write(s, &SnakesAmount, sizeof(SnakesAmount));
	// std::cout << "sent SnakesAmount " << SnakesAmount << '\n';
	write(s, &FruitsAmount, sizeof(FruitsAmount));
	// std::cout << "sent FruitsAmount " << FruitsAmount << '\n';
	for(int i = 0; i < tfa; i++){
		write(s, &FRUITSARR[i], sizeof(FRUITSARR[i]));
		// std::cout << "sent fruit coordinate of fruit " << i << ':' << FRUITSARR[i].first << ' ' << FRUITSARR[i].second << '\n';
	}
	for(int i = 0; i < tsa; i++){
		SNAKES[i]->sendStatic(s);
		// std::cout << "sent snake static " << i << '\n';
		SNAKES[i]->sendDynamic(s);
		// std::cout << "sent snake dynamic " << i << '\n';
		// std::cout << "sent snake " << i << '\n';
	}
}

void SnakeGame::getStatic(int s){
}

void SnakeGame::getDynamic(int s){
}

void SnakeGame::getSnake(int s, int i){
}

void SnakeGame::sendStatic(int s){
}

void SnakeGame::sendDynamic(int s){
}
void SnakeGame::sendSnake(int s, int i){

}

void SnakeGame::Draw(sf::RenderWindow* w, int msnakeid){
	w->clear(sf::Color::White);
	for(int i = 0; i < FruitsAmount; i++){
		sf::RectangleShape cell;
		cell.setSize(sf::Vector2f(20, 20));
		cell.setFillColor(sf::Color::Red);
		cell.setPosition(FRUITSARR[i].first * 20, FRUITSARR[i].second * 20);
		w->draw(cell);
	}

	for(int i = 0; i < SnakesAmount; i++)
		SNAKES[i]->Draw(w, SNAKES[i]->getId()==msnakeid);
	w->display();
}

int SnakeGame::tryEat(int x, int y){
	for(int i = 0; i < FruitsAmount; i++){
		if(FRUITSARR[i] == std::make_pair(x, y)){
			FruitsAmount--;
			std::pair<int, int>* tmp = new std::pair<int, int>[FruitsAmount];
			int o = 0;
			for(int i = 0; i < FruitsAmount + 1; i++){
				if(FRUITSARR[i] == std::make_pair(x, y)){
					o = 1;
					continue;
				}
				tmp[i - o] = FRUITSARR[i];
			}
			delete[] FRUITSARR;
			FRUITSARR = tmp;
			AddFruit();
			return 1;
		}
	}
	return 0;
}

int SnakeGame::getIndex(int i){
	for(int j = 0; j < SnakesAmount; j++){
		if(SNAKES[j]->getId() == i)
			return j;
	}
	return -1;
}

void SnakeGame::sendSnakeDir(int sock, Directon dir, int id){
	int sig = 3;
	write(sock, &sig, sizeof(3));
	// std::cout << "signal sent: 3\n";
	write(sock, &id, sizeof(id));
	write(sock, &dir, sizeof(dir));
}

void SnakeGame::getSnakeDir(int sock){
	int id;
	read(sock, &id, sizeof(id));
	read(sock, &SNAKES[getIndex(id)]->DIR, sizeof(Directon));
}

int SnakeGame::addSnake(){
	AddFruit();	
	Snake** tmp = new Snake*[SnakesAmount];
	for(int i = 0; i < SnakesAmount; i++){
		tmp[i] = SNAKES[i];
	}
	tmp[SnakesAmount] = new Snake(this, SnakesAmount + 1);
	delete[] SNAKES;
	SNAKES = tmp;
	SnakesAmount++;
	return SNAKES[SnakesAmount - 1]->getId();
}

void SnakeGame::Step(){
	for(int i = 0; i < SnakesAmount; i++)
		SNAKES[i]->Update();
}

std::pair<int, int> SnakeGame::getSize(){
	return {W, H};
}

void Snake::getStatic(int s){
	read(s, &DIR, sizeof(DIR));
	// std::cout << "got DIR " << DIR << '\n';
	read(s, &ID, sizeof(ID));
	// std::cout << "got ID " << ID << '\n';
	read(s, &POS, sizeof(POS));
	// std::cout << "got POS " << POS.first << ' ' << POS.second << '\n';
	read(s, &LEN, sizeof(LEN));
	// std::cout << "got LEN " << LEN << '\n';
}

void Snake::getDynamic(int s){
	delete[] PARTS;
	PARTS = new std::pair<int, int>[LEN];
	for(int i = 0; i < LEN; i++){
		read(s, &PARTS[i], sizeof(PARTS[i]));
		// std::cout << "got PART " << i << " coords " << PARTS[i].first << ' ' << PARTS[i].second << '\n';
	}
}

void Snake::sendStatic(int s){
	write(s, &DIR, sizeof(DIR));
	// std::cout << "sent DIR " << DIR << '\n';
	write(s, &ID, sizeof(ID));
	// std::cout << "sent ID " << ID << '\n';
	write(s, &POS, sizeof(POS));
	// std::cout << "sent POS " << POS.first << ' '  << POS.second << '\n';
	write(s, &LEN, sizeof(LEN));
	// std::cout << "sent LEN " << LEN << '\n';
}

void Snake::sendDynamic(int s){
	for(int i = 0; i < LEN; i++){
		write(s, &PARTS[i], sizeof(PARTS[i]));
		// std::cout << "sent PART " << i << " coords " << PARTS[i].first << ' ' << PARTS[i].second << '\n';
	}	
}

Snake::Snake(SnakeGame* ses_ptr, int id){
	LEN = 0;
	session = ses_ptr;
	POS = {rand() % session->getSize().first, rand() % session->getSize().second};
	LPOS=POS;
	PARTS = new std::pair<int,int>[1];
	ID = id;
	AddPart();
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

void Snake::Update(){
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
	POS.first=div(POS.first, session->getSize().first);
	POS.second=div(POS.second, session->getSize().second);
	if (LEN > 0){
		std::pair<int,int> prev2;
		for (int i = 0; i < LEN; i++)
		{
			prev2 = PARTS[i];
			PARTS[i] = LPOS;
			LPOS = prev2;
		}
	}
	if(session->tryEat(POS.first, POS.second)){
		AddPart();
	}
}

void Snake::Draw(sf::RenderWindow* w, bool active){
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

void Snake::AddPart(){
	LEN++;
	std::pair<int,int>* tmp = new std::pair<int,int>[LEN];
	for(int i = 0; i < LEN - 1; i++){
		tmp[i] = PARTS[i];
	}
	tmp[LEN - 1] = {LPOS.first, LPOS.second};
	delete[] PARTS;
	PARTS = tmp;
}