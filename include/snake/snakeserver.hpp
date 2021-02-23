#include "snakebase.hpp"

class SnakeGameServer : public SnakeGameBase {
public:
    using SnakeGameBase::SnakeGameBase;
    int16_t addSnake();
    void removeSnake(int16_t id);
    void step();
    void send(int socket);
    void getSnakeDirection(int socket, int16_t id);
    void addPart(int16_t id);
    void addPart(Snake* s);
};