#ifndef ASTEROIDS_GAME_GAMESTATE_H
#define ASTEROIDS_GAME_GAMESTATE_H

class GameState {
public:
    virtual ~GameState() = default;

    virtual void handleInput() = 0;
    virtual void update(float dt) = 0;
};

#endif //ASTEROIDS_GAME_GAMESTATE_H
