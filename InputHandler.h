#ifndef ASTEROIDS_GAME_INPUTHANDLER_H
#define ASTEROIDS_GAME_INPUTHANDLER_H

#include "Command.h"
#include <memory>

class Game;

class InputHandler {
private:
    Game& game;
public:
    explicit InputHandler(Game& g);
    std::unique_ptr<Command> handle(char key, float scale = 0.0f) const;
};

#endif //ASTEROIDS_GAME_INPUTHANDLER_H