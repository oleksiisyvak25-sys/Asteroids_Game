#ifndef ASTEROIDS_GAME_INPUTHANDLER_H
#define ASTEROIDS_GAME_INPUTHANDLER_H

#include "Command.h"
#include <memory>

class GameEngine;

class InputHandler {
private:
    GameEngine& engine;

public:
    explicit InputHandler(GameEngine& gameEngine);

    std::shared_ptr<Command> handle(char key) const;
};

#endif //ASTEROIDS_GAME_INPUTHANDLER_H
