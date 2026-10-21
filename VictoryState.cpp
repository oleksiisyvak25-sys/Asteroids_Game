#include "VictoryState.h"
#include "Game.h"
#include "MenuState.h"

VictoryState::VictoryState(Game& gameInstance) : game(gameInstance) {
    game.getRenderer()->showVictory();
}

void VictoryState::handleInput() {
    const char key = game.readKey("victory> ");
    if (key == 'c' || key == 'C') {
        game.continueEndless();
    } else if (key == 'm' || key == 'M') {
        game.changeState(std::make_shared<MenuState>(game));
    } else if (key == 'q' || key == 'Q') {
        game.stop();
    }
}

void VictoryState::update(float dt) {
    (void)dt;
}
