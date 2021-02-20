#include "snakebase.hpp"

class SnakeGameServer : public SnakeGameBase {
public:
    using SnakeGameBase::SnakeGameBase;
    uint16_t addSnake();
    void removeSnake(uint16_t id);
    void step();
    void send(int socket);
    void getSnakeDirection(int socket, uint16_t id);
    void addPart(uint16_t id);
    void addPart(Snake* s);
};