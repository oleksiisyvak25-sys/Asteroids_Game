#include "ShieldBonus.h"
#include "Config.h"

ShieldBonus::ShieldBonus(Vector2 pos, Vector2 vel)
    : lifetime(Config::BONUS_LIFETIME), collected(false) {
    position = pos;
    velocity = vel;
    radius = Config::BONUS_RADIUS;
}

ObjectType ShieldBonus::getType() const {
    return ObjectType::Bonus;
}

void ShieldBonus::update(float dt) {
    GameObject::update(dt);
    lifetime -= dt;
    if (lifetime <= 0.0f) {
        active = false;
    }
}

void ShieldBonus::onCollision(GameObject& other) {
    if (other.getType() == ObjectType::Ship) {
        collected = true;
        active = false;
    }
}

bool ShieldBonus::isCollected() const {
    return collected;
}
