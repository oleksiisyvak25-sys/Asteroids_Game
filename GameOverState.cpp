#include "GameOverState.h"
#include "Game.h"
#include "MenuState.h"

GameOverState::GameOverState(Game& gameInstance) : game(gameInstance) {}

void GameOverState::handleInput() {
    for (char key : game.pressedKeys()) {
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

void GameOverState::update(float dt) {
    (void)dt;
}

void GameOverState::render() {
    game.getRenderer()->render(game.getEngine()->getObjects());
    game.getRenderer()->showGameOver();
}
