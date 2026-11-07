#include "GameObject.h"
#include "Config.h"
#include <cmath>

namespace {
    float wrapValue(float value, float limit) {
        value = std::fmod(value, limit);
        if (value < 0.0f) {
            value += limit;
        }
        return value;
    }
}

GameObject::GameObject(Vector2 pos, Vector2 vel, float r)
    : position(pos),
      velocity(vel),
      angle(0.0f),
      radius(r),
      active(true) {}

void GameObject::update(float dt) {
    position += velocity * dt;
    wrapAround();
}

void GameObject::wrapAround() {
    position.x = wrapValue(position.x, Config::WORLD_WIDTH);
    position.y = wrapValue(position.y, Config::WORLD_HEIGHT);
}

const Vector2& GameObject::getPosition() const { return position; }
const Vector2& GameObject::getVelocity() const { return velocity; }
float GameObject::getAngle() const { return angle; }
float GameObject::getRadius() const { return radius; }
bool GameObject::isActive() const { return active; }
void GameObject::activate() { active = true; }

void GameObject::deactivate() { active = false; }
