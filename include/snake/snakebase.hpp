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
#include <math.h> // ..

enum direction { STOP, LEFT, RIGHT, UP, DOWN};

template <typename T> 
T getRandomReal(T a, T b) 
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_real_distribution<T> distrib(a, b);
    return distrib(randomGenerator);
}

template <typename T, typename K>
struct vec2 {
    vec2() = default;
    vec2(T _x, K _y) : x(_x), y(_y) {};
    vec2(T _x) : x(_x), y(_x) {};
    T x;
    K y;
    bool operator==(vec2 other){
        return (this->x == other.x  && this->y == other.y);
    }
    void operator+=(vec2 other){
        this->x += other.x;
        this->y += other.y;
    }
};

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct SnakePart {
    SnakePart() = default;
    SnakePart(vec2<float,float> h, vec2<float,float> b, vec2<float,float> vh, vec2<float,float> vb);
    vec2<float,float> head;
    vec2<float,float> back;
    vec2<float,float> vecHead;
    vec2<float,float> vecBack;
};

struct Snake{
    Snake();
    Snake(int16_t i, float x, float y);
    bool alive;
    direction dir;
    int16_t id, lenght;
    float step;
    std::pair<float, float> pos, lpos;
    SnakePart* parts;
};

class SnakeGameBase {
public:
    SnakeGameBase() = default;
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