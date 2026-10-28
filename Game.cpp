#include "Game.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "Config.h"
#include <algorithm>
#include <chrono>
#include <iostream>

Game::Game(const std::string& highScoreFile) : isRunning(true) {
    engine = std::make_shared<GameEngine>();
    scoreManager = std::make_shared<ScoreManager>(highScoreFile);
    renderer = std::make_shared<Renderer>(scoreManager);

    engine->subscribe(scoreManager);
    engine->subscribe(renderer);

    state = std::make_shared<MenuState>(*this);
}

void Game::changeState(const std::shared_ptr<GameState>& newState) {
    pendingState = newState;
}

bool Game::applyPendingState() {
    if (!pendingState) {
        return false;
    }
    state = std::move(pendingState);
    pendingState.reset();
    return true;
}

void Game::stop() {
    isRunning = false;
}

void Game::startNewGame() {
    engine->reset();
    changeState(std::make_shared<PlayingState>(*this));
}

void Game::continueEndless() {
    engine->enableEndlessMode();
    changeState(std::make_shared<PlayingState>(*this));
}

std::vector<char> Game::pressedKeys() {
    return renderer->takePressedKeys();
}

bool Game::isKeyHeld(char key) const {
    return renderer->isKeyHeld(key);
}

void Game::run() {
    using Clock = std::chrono::steady_clock;
    auto last = Clock::now();

    while (isRunning && renderer->isOpen() && state) {
        const auto now = Clock::now();
        const float dt = std::min(std::chrono::duration<float>(now - last).count(),
                                  Config::MAX_FRAME_TIME);
        last = now;

        renderer->pollEvents();
        if (!renderer->isOpen()) {
            break;
        }

        state->handleInput();
        if (!isRunning) {
            break;
        }
        applyPendingState();

        state->update(dt);
        applyPendingState();

        renderer->beginFrame();
        state->render();
        renderer->endFrame();
    }
    std::cout << "\n[Game] Main loop terminated. Bye!\n";
}

std::shared_ptr<GameEngine> Game::getEngine() const { return engine; }
std::shared_ptr<Renderer> Game::getRenderer() const { return renderer; }
std::shared_ptr<ScoreManager> Game::getScoreManager() const { return scoreManager; }
