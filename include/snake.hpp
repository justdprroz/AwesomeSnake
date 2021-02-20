#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <SFML/Graphics.hpp>
#include <iostream>

// Direction enumeration for snake rotation
enum DIRECTION { S = 0, L, R, U, D, K};

// Defining classes
class SnakeGame;
class Snake;

// Declaring classes 
class SnakeGame{
public:
    // Constructor
    SnakeGame();
    // Method to add new snake
    // Returns snake ID
    int addSnake();
    // Method to remove snake
    // Takes snake ID
    void removeSnake(int id);
    // Method to draw
    // Takes SFML window pointer and current snake ID to highlight it
    void draw(sf::RenderWindow* w, int msnakeid);
    // Main update method
    void step();
    // Method to try eat fruit
    // return 1 if eaten and 0 if not
    int tryEat(int x, int y);
    // Method to add fruit on field
    void addFruit();
    // Methid to get game field sizes
    // Return W and H
    std::pair<int, int> getSize();
    // Method to get memory pointer to snake of id
    // Takes snake ID
    // Returns Snake* pointer
    Snake* getSnakePtr(int id);
    // Method to get pointer to this SnakeGame instance
    // Returns SnakeGame* pointer
    SnakeGame* getSessionPtr();
    // Method to get index in SNAKES array to snake with id ID
    // Takes snake ID
    // Returns snake index
    int getIndex(int id);
    // Method to send all data to server
    // Takes socket
    void send(int s);
    // Method to get all data from client
    // Takes socket
    void get(int s);  
    // Method to send snake direction to server 
    // Takes socket and snake ID
    void sendSnakeDir(int sock, int id);
    // Method to get snake direction from client
    // Takes socket
    void getSnakeDir(int sock);
private:
    // Width and Height of game field
    int W, H;
    // Amount of Snakes and Fruits
    int SnakesAmount, FruitsAmount;
    // Pointer to array of pointers to Snake(s)
    Snake** SNAKES;
    // Pointer to array of pairs with coords
    std::pair<int, int>* FRUITSARR;
    // Locker to avoid merge communication and logic
    bool LOCK;
};

class Snake{
public:
    // Snake construstor
    // Takes pointer to SnakeGame instance and ID of snakes
    Snake(SnakeGame* ses_ptr, int id);
    // Method to update snakes POS, tails' POS, LEN etc
    void update();
    // Method to draw snake
    // Takes window pointer and active bool for highlight
    void draw(sf::RenderWindow* w, bool active);
    // Method to process events
    // Takes SFML event
    void handleEvents(sf::Event e);
    // Method to add part to sneak
    void addPart();
    // Method to get static data from client
    // Takes socket
    void getStatic(int s);
    // Method to get Dynamic data from client
    // Takes socket
    void getDynamic(int s);
    // Method to send static data to server
    // Takes socket
    void sendStatic(int s);
    // Method to send dynamic data to server
    // Takes socket
    void sendDynamic(int s);
    // Method to return snake ID
    // Returns ID
    int getId();
    // Method to set snake direction to given
    // Takes direction
    void setDir(DIRECTION d);
    // Method to get snake direction
    // Returns direction
    DIRECTION getDir();
private:
    // Direction of snake
    DIRECTION DIR;
    // ID of snake
    int ID;
    // Is snake alive?
    bool ALIVE;
    // Coords of head
    std::pair<int,int> POS;
    // Previous coords of head
    std::pair<int,int> LPOS;
    // Lenght of snake
    int LEN;
    // Array of parts coords
    std::pair<int,int>* PARTS;
    // Pointer to SnakeGame instance
    SnakeGame* SESSION;
};