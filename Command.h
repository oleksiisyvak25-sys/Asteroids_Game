#ifndef ASTEROIDS_GAME_COMMAND_H
#define ASTEROIDS_GAME_COMMAND_H

class Spaceship;
class GameEngine;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Spaceship& ship) = 0;
};

class RotateCommand : public Command {
private:
    float direction;

public:
    explicit RotateCommand(float dir);
    void execute(Spaceship& ship) override;
};

class ThrustCommand : public Command {
private:
    float scale;

public:
    explicit ThrustCommand(float s = 1.0f);
    void execute(Spaceship& ship) override;
};

class ShootCommand : public Command {
private:
    GameEngine& engine;

public:
    explicit ShootCommand(GameEngine& gameEngine);
    void execute(Spaceship& ship) override;
};

#endif //ASTEROIDS_GAME_COMMAND_H
