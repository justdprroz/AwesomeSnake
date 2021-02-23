#include "snakebase.hpp"
#include <SFML/Graphics.hpp>

class SnakeGameClient : public SnakeGameBase {
public:
    using SnakeGameBase::SnakeGameBase;
    void draw(sf::RenderWindow* w, int as);
    void get(int socket);
    void sendSnakeDir(int socket, int id);
    void handleEvents(sf::Event e, int16_t id);
};