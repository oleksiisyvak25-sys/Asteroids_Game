#ifndef ASTEROIDS_GAME_PROJECTILE_H
#define ASTEROIDS_GAME_PROJECTILE_H

#include "GameObject.h"

class Projectile : public GameObject {
private:
    float lifetime;
    float maxLifetime;

public:
    Projectile();
    ~Projectile() override = default;

    ObjectType getType() const override;
    void update(float dt) override;
    void onCollision(GameObject& other) override;

    void reset();
    void init(Vector2 pos, Vector2 vel);
    float getLifetime() const;
};

#endif //ASTEROIDS_GAME_PROJECTILE_H
