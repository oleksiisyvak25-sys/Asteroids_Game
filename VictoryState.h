#ifndef ASTEROIDS_GAME_VICTORYSTATE_H
#define ASTEROIDS_GAME_VICTORYSTATE_H

#include "GameState.h"

class Game;

class VictoryState : public GameState {
private:
    Game& game;

public:
    explicit VictoryState(Game& gameInstance);
    ~VictoryState() override = default;

    void handleInput() override;
    void update(float dt) override;
    void render() override;
};

#endif //ASTEROIDS_GAME_VICTORYSTATE_H
