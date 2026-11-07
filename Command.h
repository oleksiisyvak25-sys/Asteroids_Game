#ifndef ASTEROIDS_GAME_COMMAND_H
#define ASTEROIDS_GAME_COMMAND_H

class GameEngine;
class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(GameEngine& engine) = 0;
};

class RotateCommand : public Command {
private:
    float direction;
public:
    explicit RotateCommand(float dir);
    void execute(GameEngine& engine) override;
};

class ThrustCommand : public Command {
private:
    float scale;
public:
    explicit ThrustCommand(float s);
    void execute(GameEngine& engine) override;
};

class ShootCommand : public Command {
public:
    ShootCommand() = default;
    void execute(GameEngine& engine) override;
};

class PauseCommand : public Command {
private:
    Game& game;
public:
    explicit PauseCommand(Game& g);
    void execute(GameEngine& engine) override;
};

class QuitCommand : public Command {
private:
    Game& game;
public:
    explicit QuitCommand(Game& g);
    void execute(GameEngine& engine) override;
};

#endif //ASTEROIDS_GAME_COMMAND_H