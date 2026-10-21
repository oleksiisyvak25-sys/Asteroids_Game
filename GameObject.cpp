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

GameObject::GameObject()
    : position(0.0f, 0.0f),
      velocity(0.0f, 0.0f),
      angle(0.0f),
      radius(10.0f),
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

void GameObject::setPosition(const Vector2& p) { position = p; }
void GameObject::setVelocity(const Vector2& v) { velocity = v; }
void GameObject::deactivate() { active = false; }
