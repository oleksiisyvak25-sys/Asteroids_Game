#include "Spaceship.h"
#include "Config.h"
#include <algorithm>
#include <cmath>

Spaceship::Spaceship(ProjectilePool& projectilePool)
    : health(Config::SHIP_START_HEALTH),
      shield(false),
      shieldTimer(0.0f),
      shootTimer(0.0f),
      pool(projectilePool) {
    position = Vector2(Config::WORLD_WIDTH / 2.0f, Config::WORLD_HEIGHT / 2.0f);
    angle = -90.0f;
    radius = Config::SHIP_RADIUS;
}

ObjectType Spaceship::getType() const {
    return ObjectType::Ship;
}

void Spaceship::clampSpeed() {
    velocity = velocity.clampedLength(Config::SHIP_MAX_SPEED);
}

void Spaceship::update(float dt) {
    clampSpeed();
    GameObject::update(dt);

    if (shield) {
        shieldTimer -= dt;
        if (shieldTimer <= 0.0f) {
            shield = false;
            shieldTimer = 0.0f;
        }
    }
    if (shootTimer > 0.0f) {
        shootTimer = std::max(0.0f, shootTimer - dt);
    }
}

void Spaceship::onCollision(GameObject& other) {
    switch (other.getType()) {
        case ObjectType::Asteroid:
            takeDamage(Config::ASTEROID_DAMAGE);
            break;
        case ObjectType::Bonus:
            activateShield(Config::SHIELD_BONUS_DURATION);
            break;
        default:
            break;
    }
}

void Spaceship::rotate(float dir) {
    angle += dir * Config::SHIP_ROTATION_STEP;
    angle = std::fmod(angle, 360.0f);
    if (angle > 180.0f) angle -= 360.0f;
    if (angle <= -180.0f) angle += 360.0f;
}

void Spaceship::thrust(float scale) {
    velocity += Vector2::fromAngle(angle) * (Config::SHIP_THRUST_IMPULSE * scale);
    clampSpeed();
}

std::shared_ptr<Projectile> Spaceship::shoot() {
    if (shootTimer > 0.0f) {
        return nullptr;
    }

    const Vector2 dir = Vector2::fromAngle(angle);
    const Vector2 nose = position + dir * (radius + Config::PROJECTILE_RADIUS + 1.0f);
    const Vector2 projectileVelocity = velocity + dir * Config::PROJECTILE_SPEED;

    auto projectile = pool.acquire();
    projectile->init(nose, projectileVelocity);
    shootTimer = Config::SHIP_SHOOT_COOLDOWN;
    return projectile;
}

void Spaceship::takeDamage(int damage) {
    if (shield || health <= 0) {
        return;
    }
    health -= damage;
    if (health > 0) {
        activateShield(Config::SHIP_HIT_INVULNERABILITY);
    }
}

void Spaceship::activateShield(float seconds) {
    shield = true;
    shieldTimer = std::max(shieldTimer, seconds);
}

void Spaceship::addVelocity(const Vector2& dv) {
    velocity += dv;
}

int Spaceship::getHealth() const { return health; }
bool Spaceship::isShielded() const { return shield; }
float Spaceship::getShieldTime() const { return shieldTimer; }
