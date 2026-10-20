//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_SPACESHIP_H
#define ASTEROIDS_GAME_SPACESHIP_H

#include "GameObject.h"
#include "ProjectilePool.h"
#include <memory>

class Spaceship : public GameObject {
public:
    int health;
    bool shield;

    Spaceship();
    ~Spaceship() override = default;

    ObjectType getType() const override;
    void rotate(float dir);
    void thrust();
    std::shared_ptr<Projectile> shoot(ProjectilePool& pool);
    void onCollision(GameObject* other) override;
};


#endif //ASTEROIDS_GAME_SPACESHIP_H
