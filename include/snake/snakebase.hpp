#include <unistd.h> // read() write() close()
#include <sys/socket.h> // socket
#include <arpa/inet.h> // address control

#include <cstdint> // int16_t
#include <utility> // pairs
#include <random> // random
#include <thread> // thread
#include <chrono> // timeformats
#include <mutex> // mutex
#include <iostream> // ...


enum direction { STOP, LEFT, RIGHT, UP, DOWN};

template <typename T> 
T getRandomReal(T a, T b) 
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_real_distribution<T> distrib(a, b);
    return distrib(randomGenerator);
} 

struct Snake{
    Snake();
    Snake(int16_t i, int16_t w, int16_t h);
    Snake(int16_t i, float x, float y);
    bool alive;
    direction dir;
    int16_t id, lenght;
    float step;
    std::pair<float, float> pos, lpos;
    std::pair<float, float>* parts;
};

class SnakeGameBase {
public:
    SnakeGameBase(int16_t msa, int16_t fa);
    SnakeGameBase(int16_t w, int16_t h, int16_t msa, int16_t fa);
    int16_t getSnakeIndexById(int16_t id);
    int16_t getSnakeId(Snake* s);
protected:
    int16_t width, height;
    int16_t snakesAmount, fruitsAmount, maxSnakesAmount;
    Snake** snakes;
    std::pair<float, float>* fruits;
};