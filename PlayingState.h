#ifndef ASTEROIDS_GAME_PLAYINGSTATE_H
#define ASTEROIDS_GAME_PLAYINGSTATE_H

#include "GameState.h"
#include "InputHandler.h"
#include <memory>

class Game;

class PlayingState : public GameState {
private:
    Game& game;
    std::unique_ptr<InputHandler> inputHandler;

public:
    explicit PlayingState(Game& gameInstance);
    ~PlayingState() override = default;

    void handleInput() override;
    void update(float dt) override;
};

#endif //ASTEROIDS_GAME_PLAYINGSTATE_H
