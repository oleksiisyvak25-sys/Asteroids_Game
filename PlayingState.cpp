#include "PlayingState.h"
#include "Game.h"
#include "PausedState.h"
#include "GameOverState.h"
#include "VictoryState.h"

PlayingState::PlayingState(Game& gameInstance) : game(gameInstance) {
    inputHandler = std::make_unique<InputHandler>(*game.getEngine());
    game.getRenderer()->render(game.getEngine()->getObjects()); // показати поле одразу
}

void PlayingState::handleInput() {
    std::string line;
    if (!game.readLine("[W/A/D move, Space/F shoot, P pause, Q quit, Enter = wait] > ", line)) {
        return;
    }

    auto engine = game.getEngine();
    for (char key : line) {
        if (key == 'p' || key == 'P') {
            game.changeState(std::make_shared<PausedState>(game));
            return;
        }
        if (key == 'q' || key == 'Q') {
            game.stop();
            return;
        }

        std::shared_ptr<Command> command = inputHandler->handle(key);
        if (command) {
            command->execute(*engine->getShip());
        }
    }
}

void PlayingState::update(float dt) {
    auto engine = game.getEngine();
    engine->update(dt);
    game.getRenderer()->render(engine->getObjects());

    if (engine->isGameOver()) {
        game.changeState(std::make_shared<GameOverState>(game));
    } else if (engine->isVictory()) {
        game.changeState(std::make_shared<VictoryState>(game));
    }
}
