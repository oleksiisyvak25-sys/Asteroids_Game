#include "PlayingState.h"
#include "Game.h"
#include "Config.h"
#include "PausedState.h"
#include "GameOverState.h"
#include "VictoryState.h"

PlayingState::PlayingState(Game& gameInstance) : game(gameInstance) {
    inputHandler = std::make_unique<InputHandler>(*game.getEngine());
}

void PlayingState::handleInput() {
    for (char key : game.pressedKeys()) {
        if (key == 'p') {
            game.changeState(std::make_shared<PausedState>(game));
            return;
        }
        if (key == 'q') {
            game.stop();
            return;
        }
    }
}

void PlayingState::applyHeldControls(float dt) {
    auto engine = game.getEngine();
    const float scale = dt / Config::TURN_TIME;

    for (char key : {'a', 'd', 'w', ' '}) {
        if (!game.isKeyHeld(key)) {
            continue;
        }
        std::shared_ptr<Command> command = inputHandler->handle(key, scale);
        if (command) {
            command->execute(*engine->getShip());
        }
    }
}

void PlayingState::update(float dt) {
    auto engine = game.getEngine();
    applyHeldControls(dt);
    engine->update(dt);

    if (engine->isGameOver()) {
        game.changeState(std::make_shared<GameOverState>(game));
    } else if (engine->isVictory()) {
        game.changeState(std::make_shared<VictoryState>(game));
    }
}

void PlayingState::render() {
    game.getRenderer()->render(game.getEngine()->getObjects());
}
