#include "Game.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "Config.h"
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

bool Game::readLine(const std::string& prompt, std::string& line) {
    renderer->showPrompt(prompt);
    if (!std::getline(std::cin, line)) {
        stop();
        return false;
    }
    return true;
}

char Game::readKey(const std::string& prompt) {
    std::string line;
    if (!readLine(prompt, line)) {
        return '\0';
    }
    for (char c : line) {
        if (c != ' ' && c != '\t' && c != '\r') {
            return c;
        }
    }
    return '\0';
}

void Game::run() {
    while (isRunning && state) {
        state->handleInput();
        if (!isRunning) {
            break;
        }
        if (applyPendingState()) {
            continue;
        }
        state->update(Config::TURN_TIME);
        applyPendingState();
    }
    std::cout << "\n[Game] Main loop terminated. Bye!\n";
}

std::shared_ptr<GameEngine> Game::getEngine() const { return engine; }
std::shared_ptr<Renderer> Game::getRenderer() const { return renderer; }
std::shared_ptr<ScoreManager> Game::getScoreManager() const { return scoreManager; }
