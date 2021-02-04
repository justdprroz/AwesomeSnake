#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <SFML/Graphics.hpp>
#include <iostream>
enum Directon { S = 0, L, R, U, D};
class SnakeGame;
class Snake;
class SnakeGame{
public:
    SnakeGame();
    int addSnake();
    void removeSnake(Snake* S_ptr);
    void Draw(sf::RenderWindow* w, int msnakeid);
    void Step();
    int tryEat(int x, int y);
    void AddFruit();
    std::pair<int, int> getSize();
    Snake* getsnakeptr(int id);
    SnakeGame* getsessionptr();
    int getIndex(int i);
    void send(int s);
    void get(int s);
    void getStatic(int s);
    void getDynamic(int s);
    void getSnake(int s, int i);
    void sendStatic(int s);
    void sendDynamic(int s);
    void sendSnake(int s, int i);    
    void sendSnakeDir(int sock, Directon d, int id);
    void getSnakeDir(int sock);
    Snake** SNAKES;
private:
    int W, H;
    int SnakesAmount;
    int FruitsAmount;
    std::pair<int, int>* FRUITSARR;
};

class Snake{
public:
    Snake(SnakeGame* ses_ptr, int id);
    void Move();
    void Update();
    void Draw(sf::RenderWindow* w, bool active);
    void handleEvents(sf::Event e);
    void AddPart();
    void getStatic(int s);
    void getDynamic(int s);
    void sendStatic(int s);
    void sendDynamic(int s);
    int getId();
    Directon DIR;
private:
    int ID;
    bool alive;
    std::pair<int,int> POS;
    std::pair<int,int> LPOS;
    int LEN;
    std::pair<int,int>* PARTS;
    SnakeGame* session;
};