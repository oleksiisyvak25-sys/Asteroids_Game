#include "Command.h"
#include "Spaceship.h"
#include "GameEngine.h"

RotateCommand::RotateCommand(float dir) : direction(dir) {}

void RotateCommand::execute(Spaceship& ship) {
    ship.rotate(direction);
}

void ThrustCommand::execute(Spaceship& ship) {
    ship.thrust();
}

ShootCommand::ShootCommand(GameEngine& gameEngine) : engine(gameEngine) {}

void ShootCommand::execute(Spaceship& ship) {
    auto projectile = ship.shoot();
    if (projectile) {
        engine.addObject(projectile);
    }
}
