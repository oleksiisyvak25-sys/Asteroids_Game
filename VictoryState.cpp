#include "VictoryState.h"
#include "Game.h"
#include "MenuState.h"

VictoryState::VictoryState(Game& gameInstance) : game(gameInstance) {}

void VictoryState::handleInput() {
    for (char key : game.pressedKeys()) {
        if (key == 'c') {
            game.continueEndless();
            return;
        }
        if (key == 'm') {
            game.changeState(std::make_shared<MenuState>(game));
            return;
        }
        if (key == 'q') {
            game.stop();
            return;
        }
    }
}

void VictoryState::update(float dt) {
    (void)dt;
}

void VictoryState::render() {
    game.getRenderer()->render(game.getEngine()->getObjects());
    game.getRenderer()->showVictory();
}
