#include "ShieldBonus.h"
#include "Config.h"

ShieldBonus::ShieldBonus(Vector2 pos, Vector2 vel)
    : GameObject(pos, vel, Config::BONUS_RADIUS),
      lifetime(Config::BONUS_LIFETIME),
      collected(false) {
}

ObjectType ShieldBonus::getType() const {
    return ObjectType::Bonus;
}

void ShieldBonus::update(float dt) {
    GameObject::update(dt);
    lifetime -= dt;
    if (lifetime <= 0.0f) {
        deactivate();
    }
}

void ShieldBonus::onCollision(GameObject& other) {
    if (other.getType() == ObjectType::Ship) {
        collected = true;
        deactivate();
    }
}

bool ShieldBonus::isCollected() const {
    return collected;
}