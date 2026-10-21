#include "MenuState.h"
#include "Game.h"

MenuState::MenuState(Game& gameInstance) : game(gameInstance) {
    game.getRenderer()->showMenu();
}

void MenuState::handleInput() {
    const char key = game.readKey("menu> ");
    if (key == 's' || key == 'S') {
        game.startNewGame();
    } else if (key == 'q' || key == 'Q') {
        game.stop();
    }
}

void MenuState::update(float dt) {
    (void)dt;
}
