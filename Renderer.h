#ifndef ASTEROIDS_GAME_RENDERER_H
#define ASTEROIDS_GAME_RENDERER_H

#include "GameObserver.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Renderer : public GameObserver {
private:
    std::ostream& out;
    std::shared_ptr<const ScoreManager> scores;
    int currentWave;

public:
    explicit Renderer(std::shared_ptr<const ScoreManager> scoreManager, std::ostream& output = std::cout);

    void render(const std::vector<std::shared_ptr<GameObject>>& objects);
    void onEvent(const GameEvent& event) override;

    void showMenu();
    void showPaused();
    void showGameOver();
    void showVictory();
    void showPrompt(const std::string& text);
};

#endif //ASTEROIDS_GAME_RENDERER_H
