#ifndef ASTEROIDS_GAME_PAUSEDSTATE_H
#define ASTEROIDS_GAME_PAUSEDSTATE_H

#include "GameState.h"

class Game;

class PausedState : public GameState {
private:
    Game& game;

public:
    explicit PausedState(Game& gameInstance);
    ~PausedState() override = default;

    void handleInput() override;
    void update(float dt) override;
};

#endif //ASTEROIDS_GAME_PAUSEDSTATE_H
