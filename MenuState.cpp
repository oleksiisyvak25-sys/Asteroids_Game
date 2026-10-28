#include "MenuState.h"
#include "Game.h"

MenuState::MenuState(Game& gameInstance) : game(gameInstance) {}

void MenuState::handleInput() {
    for (char key : game.pressedKeys()) {
        if (key == 's') {
            game.startNewGame();
            return;
        }
        if (key == 'q') {
            game.stop();
            return;
        }
    }
}

void MenuState::update(float dt) {
    (void)dt;
}

void MenuState::render() {
    game.getRenderer()->showMenu();
}
