#include "PlayingState.h"
#include "Game.h"
#include "Config.h"
#include "PausedState.h"
#include "GameOverState.h"
#include "VictoryState.h"

PlayingState::PlayingState(Game& gameInstance) : game(gameInstance) {
    inputHandler = std::make_unique<InputHandler>(game);
}

void PlayingState::handleInput() {
    for (char key : game.pressedKeys()) {
        std::unique_ptr<Command> command = inputHandler->handle(key, 0.0f);
        if (command) {
            command->execute(*game.getEngine());
        }
    }
}

void PlayingState::applyHeldControls(float dt) {
    std::shared_ptr<GameEngine> engine = game.getEngine();
    const float scale = dt / Config::TURN_TIME;

    for (char key : {'a', 'd', 'w', ' '}) {
        if (!game.isKeyHeld(key)) {
            continue;
        }
        std::unique_ptr<Command> command = inputHandler->handle(key, scale);
        if (command) {
            command->execute(*engine);
        }
    }
}

void PlayingState::update(float dt) {
    std::shared_ptr<GameEngine> engine = game.getEngine();
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