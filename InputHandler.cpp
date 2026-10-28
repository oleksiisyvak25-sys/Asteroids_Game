#include "InputHandler.h"

InputHandler::InputHandler(GameEngine& gameEngine) : engine(gameEngine) {}

std::shared_ptr<Command> InputHandler::handle(char key, float scale) const {
    switch (key) {
        case 'w':
        case 'W':
            return std::make_shared<ThrustCommand>(scale);
        case 'a':
        case 'A':
            return std::make_shared<RotateCommand>(-scale);
        case 'd':
        case 'D':
            return std::make_shared<RotateCommand>(scale);
        case ' ':
        case 'f':
        case 'F':
            return std::make_shared<ShootCommand>(engine);
        default:
            return nullptr;
    }
}
