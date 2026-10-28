#ifndef ASTEROIDS_GAME_GAME_H
#define ASTEROIDS_GAME_GAME_H

#include "GameState.h"
#include "GameEngine.h"
#include "ScoreManager.h"
#include "Renderer.h"
#include <memory>
#include <string>
#include <vector>

class Game {
private:
    std::shared_ptr<GameState> state;
    std::shared_ptr<GameState> pendingState;
    std::shared_ptr<GameEngine> engine;
    std::shared_ptr<ScoreManager> scoreManager;
    std::shared_ptr<Renderer> renderer;
    bool isRunning;

    bool applyPendingState();

public:
    explicit Game(const std::string& highScoreFile = "highscore.txt");
    ~Game() = default;

    void run();
    void changeState(const std::shared_ptr<GameState>& newState);
    void stop();

    void startNewGame();
    void continueEndless();

    std::vector<char> pressedKeys();
    bool isKeyHeld(char key) const;

    std::shared_ptr<GameEngine> getEngine() const;
    std::shared_ptr<Renderer> getRenderer() const;
    std::shared_ptr<ScoreManager> getScoreManager() const;
};

#endif //ASTEROIDS_GAME_GAME_H
