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
private:
    Vector2 position;
    Vector2 velocity;
    float angle;
    float radius;
    bool active;

    void wrapAround();

protected:
    GameObject(Vector2 pos, Vector2 vel, float r);

    void setInternalPosition(const Vector2& p) { position = p; }
    void setInternalVelocity(const Vector2& v) { velocity = v; }
    void setInternalAngle(float a) { angle = a; }
    void setInternalRadius(float r) { radius = r; }

public:
    virtual ~GameObject() = default;

    virtual ObjectType getType() const = 0;
    virtual void update(float dt);
    virtual void onCollision(GameObject& other) = 0;

    virtual void onBlast(int dmg) {}
    void activate();
    const Vector2& getPosition() const;
    const Vector2& getVelocity() const;
    float getAngle() const;
    float getRadius() const;
    bool isActive() const;

    void deactivate();

};

#endif //ASTEROIDS_GAME_GAMEOBJECT_H