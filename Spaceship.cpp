#include "Spaceship.h"
#include "Config.h"
#include <algorithm>
#include <cmath>

Spaceship::Spaceship(ProjectilePool& projectilePool)
    : GameObject(Vector2(Config::WORLD_WIDTH / 2.0f, Config::WORLD_HEIGHT / 2.0f),
                 Vector2(0.0f, 0.0f),
                 Config::SHIP_RADIUS),
      health(Config::SHIP_START_HEALTH),
      shield(false),
      shieldTimer(0.0f),
      shootTimer(0.0f),
      pool(projectilePool) {

    setInternalAngle(-90.0f);
}

ObjectType Spaceship::getType() const {
    return ObjectType::Ship;
}

void Spaceship::clampSpeed() {
    setInternalVelocity(getVelocity().clampedLength(Config::SHIP_MAX_SPEED));
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

void Spaceship::onBlast(int dmg) {
    takeDamage(dmg);
}

void Spaceship::rotate(float dir) {
    float newAngle = getAngle() + dir * Config::SHIP_ROTATION_STEP;
    newAngle = std::fmod(newAngle, 360.0f);
    if (newAngle > 180.0f) newAngle -= 360.0f;
    if (newAngle <= -180.0f) newAngle += 360.0f;
    setInternalAngle(newAngle);
}

void Spaceship::thrust(float scale) {
    Vector2 newVel = getVelocity() + Vector2::fromAngle(getAngle()) * (Config::SHIP_THRUST_IMPULSE * scale);
    setInternalVelocity(newVel);
    clampSpeed();
}

std::shared_ptr<Projectile> Spaceship::shoot() {
    if (shootTimer > 0.0f) {
        return nullptr;
    }

    const Vector2 dir = Vector2::fromAngle(getAngle());
    const Vector2 nose = getPosition() + dir * (getRadius() + Config::PROJECTILE_RADIUS + 1.0f);
    const Vector2 projectileVelocity = getVelocity() + dir * Config::PROJECTILE_SPEED;

    std::shared_ptr<Projectile> projectile = pool.acquire();
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
    setInternalVelocity(getVelocity() + dv);
}

int Spaceship::getHealth() const { return health; }
bool Spaceship::isShielded() const { return shield; }
float Spaceship::getShieldTime() const { return shieldTimer; }
