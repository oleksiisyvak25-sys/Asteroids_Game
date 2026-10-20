//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_PROJECTILE_H
#define ASTEROIDS_GAME_PROJECTILE_H

#include "GameObject.h"

class Projectile : public GameObject {
public:
    float lifetime;
    float maxLifetime;

    Projectile();
    ~Projectile() override = default;

    ObjectType getType() const override;
    void init(Vector2 pos, Vector2 vel);
    void update(float dt) override;
    void onCollision(GameObject* other) override;
};


#endif //ASTEROIDS_GAME_PROJECTILE_H
