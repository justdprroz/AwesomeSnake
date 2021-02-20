#include <unistd.h> // read() write() close()
#include <sys/socket.h> // socket
#include <arpa/inet.h> // address control

#include <cstdint> // uint16_t
#include <utility> // pairs
#include <random> // random
#include <thread> // thread
#include <chrono> // timeformats
#include <mutex> // mutex
#include <iostream> // ...


enum direction { STOP, LEFT, RIGHT, UP, DOWN};

template <typename T> 
T getRandom(T a, T b) 
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_real_distribution<T> distrib(a, b);
    return distrib(randomGenerator);
} 

struct Snake{
    Snake();
    Snake(uint16_t i, uint16_t w, uint16_t h);
    Snake(uint16_t i, double x, double y);
    bool alive;
    direction dir;
    uint16_t id, lenght;
    double step;
    std::pair<double, double> pos, lpos;
    std::pair<double, double>* parts;
};

class SnakeGameBase {
public:
    SnakeGameBase(uint16_t msa, uint16_t fa);
    SnakeGameBase(uint16_t w, uint16_t h, uint16_t msa, uint16_t fa);
    uint16_t getSnakeIndexById(uint16_t id);
    uint16_t getSnakeId(Snake* s);
protected:
    uint16_t width, height;
    uint16_t snakesAmount, fruitsAmount, maxSnakesAmount;
    Snake** snakes;
    std::pair<double, double>* fruits;
};