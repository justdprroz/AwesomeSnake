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

#include <utils.hpp>

enum direction { STOP, LEFT, RIGHT, UP, DOWN};

struct SnakePart {
    SnakePart() = default;
    SnakePart(vec2f h, vec2f b, vec2f vh, vec2f vb);
    vec2f head;
    vec2f back;
    vec2f vecHead;
    vec2f vecBack;
};

struct Snake{
    Snake();
    Snake(int16_t i, float x, float y);
    Snake(int16_t i, vec2f p);
    Snake(int16_t i, float s, float g, vec2f p);
    bool alive;
    direction dir, sdir;
    int16_t id, lenght;
    float step;
    float growth;
    vec2f pos, lpos;
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
    vec2f* fruits;
};