#include "Command.h"
#include "Spaceship.h"
#include "GameEngine.h"
#include "Game.h"
#include "PausedState.h"

RotateCommand::RotateCommand(float dir) : direction(dir) {}

void RotateCommand::execute(GameEngine& engine) {
    std::shared_ptr<Spaceship> ship = std::const_pointer_cast<Spaceship>(engine.getShip());
    if (ship) {
        ship->rotate(direction);
    }
}

ThrustCommand::ThrustCommand(float s) : scale(s) {}

void ThrustCommand::execute(GameEngine& engine) {
    std::shared_ptr<Spaceship> ship = std::const_pointer_cast<Spaceship>(engine.getShip());
    if (ship) {
        ship->thrust(scale);
    }
}

void ShootCommand::execute(GameEngine& engine) {
    std::shared_ptr<Spaceship> ship = std::const_pointer_cast<Spaceship>(engine.getShip());
    if (ship) {
        std::shared_ptr<Projectile> projectile = ship->shoot();
        if (projectile) {
            engine.addObject(projectile);
        }
    }
}

PauseCommand::PauseCommand(Game& g) : game(g) {}

void PauseCommand::execute(GameEngine& engine) {
    game.changeState(std::make_shared<PausedState>(game));
}

QuitCommand::QuitCommand(Game& g) : game(g) {}

void QuitCommand::execute(GameEngine& engine) {
    game.stop();
}