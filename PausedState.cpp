#include "PausedState.h"
#include "Game.h"
#include "PlayingState.h"
#include "MenuState.h"

PausedState::PausedState(Game& gameInstance) : game(gameInstance) {}

void PausedState::handleInput() {
    for (char key : game.pressedKeys()) {
        if (key == 'r' || key == 'p') {
            game.changeState(std::make_shared<PlayingState>(game));
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

void PausedState::update(float dt) {
    (void)dt;
}

void PausedState::render() {
    game.getRenderer()->render(game.getEngine()->getObjects());
    game.getRenderer()->showPaused();
}
