#include "InputHandler.h"

InputHandler::InputHandler(Game& g) : game(g) {}

std::unique_ptr<Command> InputHandler::handle(char key, float scale) const {
    switch (key) {
        case 'w': case 'W':
            return std::make_unique<ThrustCommand>(scale);
        case 'a': case 'A':
            return std::make_unique<RotateCommand>(-scale);
        case 'd': case 'D':
            return std::make_unique<RotateCommand>(scale);
        case ' ': case 'f': case 'F':
            return std::make_unique<ShootCommand>();
        case 'p': case 'P':
            return std::make_unique<PauseCommand>(game);
        case 'q': case 'Q':
            return std::make_unique<QuitCommand>(game);

        default:
            return nullptr;
    }
}