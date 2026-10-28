#ifndef ASTEROIDS_GAME_SPACESHIP_H
#define ASTEROIDS_GAME_SPACESHIP_H

#include "GameObject.h"
#include "ProjectilePool.h"
#include <memory>

class Spaceship : public GameObject {
private:
    int health;
    bool shield;
    float shieldTimer;
    float shootTimer;
    ProjectilePool& pool;

    void clampSpeed();

public:
    explicit Spaceship(ProjectilePool& projectilePool);
    ~Spaceship() override = default;

    ObjectType getType() const override;
    void update(float dt) override;
    void onCollision(GameObject& other) override;

    void rotate(float dir);
    void thrust(float scale = 1.0f);
    std::shared_ptr<Projectile> shoot();

    void takeDamage(int damage);
    void activateShield(float seconds);
    void addVelocity(const Vector2& dv);

    int getHealth() const;
    bool isShielded() const;
    float getShieldTime() const;
};

#endif //ASTEROIDS_GAME_SPACESHIP_H
