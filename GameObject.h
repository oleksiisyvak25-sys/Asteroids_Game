#ifndef ASTEROIDS_GAME_GAMEOBJECT_H
#define ASTEROIDS_GAME_GAMEOBJECT_H

#include "Vector2.h"

enum class ObjectType {
    Ship,
    Asteroid,
    Projectile,
    Bonus
};

class GameObject {
protected:
    Vector2 position;
    Vector2 velocity;
    float angle;
    float radius;
    bool active;

    void wrapAround();

public:
    GameObject();
    virtual ~GameObject() = default;

    virtual ObjectType getType() const = 0;
    virtual void update(float dt);
    virtual void onCollision(GameObject& other) = 0;

    const Vector2& getPosition() const;
    const Vector2& getVelocity() const;
    float getAngle() const;
    float getRadius() const;
    bool isActive() const;

    void setPosition(const Vector2& p);
    void setVelocity(const Vector2& v);
    void deactivate();
};

#endif //ASTEROIDS_GAME_GAMEOBJECT_H
