#ifndef ASTEROIDS_GAME_MENUSTATE_H
#define ASTEROIDS_GAME_MENUSTATE_H

#include "GameState.h"

class Game;

class MenuState : public GameState {
private:
    Game& game;

public:
    explicit MenuState(Game& gameInstance);
    ~MenuState() override = default;

    void handleInput() override;
    void update(float dt) override;
};

#endif //ASTEROIDS_GAME_MENUSTATE_H
