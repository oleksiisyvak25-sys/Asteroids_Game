#include "PausedState.h"
#include "Game.h"
#include "PlayingState.h"
#include "MenuState.h"

PausedState::PausedState(Game& gameInstance) : game(gameInstance) {
    game.getRenderer()->showPaused();
}

void PausedState::handleInput() {
    const char key = game.readKey("paused> ");
    if (key == 'r' || key == 'R') {
        game.changeState(std::make_shared<PlayingState>(game));
    } else if (key == 'm' || key == 'M') {
        game.changeState(std::make_shared<MenuState>(game));
    } else if (key == 'q' || key == 'Q') {
        game.stop();
    }
}

void PausedState::update(float dt) {
    (void)dt; // під час паузи світ не рухається
}
