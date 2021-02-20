#include <random>
#include <unistd.h> // read() write() close()
#include <sys/socket.h> // socket
#include <arpa/inet.h> // address control
#include <SFML/Graphics.hpp> // sfml for graphics
#include <iostream> // ...
#include <thread> // ..
#include <chrono> // .
#include <mutex> //

enum DIRECTION { S = 0, L, R, U, D, K};

class SnakeGame;
class Snake;

class SnakeGame{
public:
    SnakeGame();//both
    int addSnake(); //server
    void removeSnake(int id);//server
    void draw(sf::RenderWindow* w, int msnakeid);//client
    void step();//server
    int tryEat(int x, int y);//server
    void addFruit();//server
    std::pair<int, int> getSize();//both
    Snake* getSnakePtr(int id); //BAN
    SnakeGame* getSessionPtr(); //BAN
    int getIndex(int id); //both
    void send(int s); //s
    void get(int s); //c
    void sendSnakeDir(int sock, int id); //c
    void getSnakeDir(int sock); //s
private:
    int W, H;
    int SnakesAmount, FruitsAmount;
    Snake** SNAKES;
    std::pair<int, int>* FRUITSARR;
    bool LOCK;
};

class Snake{
public:
    Snake(SnakeGame* ses_ptr, int id); // b
    void update(); // ban
    void draw(sf::RenderWindow* w, bool active); // ban
    void handleEvents(sf::Event e); // ban
    void addPart(); // ban
    void getStatic(int s); // ban
    void getDynamic(int s); // ban
    void sendStatic(int s); // ban
    void sendDynamic(int s); // ban
    int getId(); // ban
    void setDir(DIRECTION d); // ban
    DIRECTION getDir(); // ban
private:
    DIRECTION DIR;
    int ID;
    bool ALIVE;
    std::pair<int,int> POS;
    std::pair<int,int> LPOS;
    int LEN;
    std::pair<int,int>* PARTS;
    SnakeGame* SESSION;
};