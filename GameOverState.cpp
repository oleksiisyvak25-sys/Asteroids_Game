#include "GameOverState.h"
#include "Game.h"
#include "MenuState.h"

GameOverState::GameOverState(Game& gameInstance) : game(gameInstance) {
    game.getRenderer()->showGameOver();
}

void GameOverState::handleInput() {
    const char key = game.readKey("game over> ");
    if (key == 'm' || key == 'M') {
        game.changeState(std::make_shared<MenuState>(game));
    } else if (key == 'q' || key == 'Q') {
        game.stop();
    }
}

void GameOverState::update(float dt) {
    (void)dt;
}
