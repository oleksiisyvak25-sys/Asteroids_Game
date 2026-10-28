#ifndef ASTEROIDS_GAME_GAMEOVERSTATE_H
#define ASTEROIDS_GAME_GAMEOVERSTATE_H

#include "GameState.h"

class Game;

class GameOverState : public GameState {
private:
    Game& game;

public:
    explicit GameOverState(Game& gameInstance);
    ~GameOverState() override = default;

    void handleInput() override;
    void update(float dt) override;
    void render() override;
};

#endif //ASTEROIDS_GAME_GAMEOVERSTATE_H
