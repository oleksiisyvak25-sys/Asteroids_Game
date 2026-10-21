#include "GameEngine.h"
#include "ShieldBonus.h"
#include "Config.h"
#include <algorithm>

GameEngine::GameEngine(unsigned seed)
    : asteroidSpawner(seed + 1u),
      rng(seed),
      wave(1),
      endless(false),
      gameOver(false),
      victory(false) {
    reset();
}

void GameEngine::reset() {
    objects.clear();
    projectilePool.releaseAll();

    wave = 1;
    endless = false;
    gameOver = false;
    victory = false;

    ship = std::make_shared<Spaceship>(projectilePool);
    addObject(ship);

    notify(GameEvent(EventType::GameStarted));
    spawnWave(1);
}


void GameEngine::addObject(const std::shared_ptr<GameObject>& o) {
    if (!o) {
        return;
    }
    if (std::find(objects.begin(), objects.end(), o) != objects.end()) {
        return;
    }
    objects.push_back(o);

    if (o->getType() == ObjectType::Projectile) {
        notify(GameEvent(EventType::ProjectileFired, 0, o->getPosition()));
    }
}

void GameEngine::removeObject(const std::shared_ptr<GameObject>& o) {
    objects.erase(std::remove(objects.begin(), objects.end(), o), objects.end());
}

void GameEngine::subscribe(const std::shared_ptr<GameObserver>& o) {
    if (o) {
        observers.push_back(o);
    }
}

void GameEngine::notify(const GameEvent& e) {
    for (auto& obs : observers) {
        if (obs) {
            obs->onEvent(e);
        }
    }
}

void GameEngine::spawnWave(int waveNumber) {
    wave = waveNumber;
    for (auto& asteroid : asteroidSpawner.spawnWave(wave)) {
        addObject(asteroid);
    }
    notify(GameEvent(EventType::WaveStarted, wave));
}

void GameEngine::enableEndlessMode() {
    if (!victory) {
        return;
    }
    endless = true;
    victory = false;
    spawnWave(wave + 1);
}

void GameEngine::update(float dt) {
    while (dt > 1e-6f && !gameOver && !victory) {
        const float stepTime = std::min(dt, Config::MAX_PHYSICS_STEP);
        step(stepTime);
        dt -= stepTime;
    }
}

void GameEngine::step(float dt) {
    const int healthBefore = ship->getHealth();

    for (auto& obj : objects) {
        if (obj->isActive()) {
            obj->update(dt);
        }
    }

    for (auto& obj : objects) {
        if (obj->isActive() && obj->getType() == ObjectType::Asteroid) {
            std::static_pointer_cast<Asteroid>(obj)->applyEffect(*ship, dt);
        }
    }

    collisionSystem.check(objects);
    processDestroyedAsteroids();
    removeInactiveObjects();

    checkShipState(healthBefore);
    if (!gameOver) {
        checkWaveCleared();
    }
}

void GameEngine::collectDestroyedAsteroids(std::vector<std::shared_ptr<Asteroid>>& out) {
    for (auto it = objects.begin(); it != objects.end();) {
        if ((*it)->getType() == ObjectType::Asteroid && !(*it)->isActive()) {
            out.push_back(std::static_pointer_cast<Asteroid>(*it));
            it = objects.erase(it);
        } else {
            ++it;
        }
    }
}

void GameEngine::processDestroyedAsteroids() {
    std::vector<std::shared_ptr<Asteroid>> queue;
    std::vector<std::shared_ptr<GameObject>> spawned;
    std::uniform_real_distribution<float> unit(0.0f, 1.0f);

    collectDestroyedAsteroids(queue);

    while (!queue.empty()) {
        std::shared_ptr<Asteroid> ast = queue.back();
        queue.pop_back();

        notify(GameEvent(EventType::AsteroidDestroyed, ast->getSize(), ast->getPosition()));

        if (ast->getBlastRadius() > 0.0f) {
            explode(*ast);
            collectDestroyedAsteroids(queue);
        }

        for (auto& fragment : ast->split()) {
            spawned.push_back(fragment);
        }

        if (unit(rng) < Config::BONUS_DROP_CHANCE) {
            const Vector2 drift = Vector2::fromAngle(unit(rng) * 360.0f) * 15.0f;
            spawned.push_back(std::make_shared<ShieldBonus>(ast->getPosition(), drift));
            notify(GameEvent(EventType::BonusDropped, 0, ast->getPosition()));
        }
    }

    for (auto& o : spawned) {
        addObject(o);
    }
}

void GameEngine::explode(const Asteroid& source) {
    const float blast = source.getBlastRadius();
    notify(GameEvent(EventType::Explosion, static_cast<int>(blast), source.getPosition()));

    for (auto& obj : objects) {
        if (!obj->isActive()) {
            continue;
        }
        const float dist = source.getPosition().distanceTo(obj->getPosition());
        if (dist > blast + obj->getRadius()) {
            continue;
        }

        if (obj->getType() == ObjectType::Ship) {
            ship->takeDamage(Config::BLAST_DAMAGE);
        } else if (obj->getType() == ObjectType::Asteroid) {
            std::static_pointer_cast<Asteroid>(obj)->destroy();
        }
    }
}

void GameEngine::removeInactiveObjects() {
    for (auto& obj : objects) {
        if (obj->isActive()) {
            continue;
        }
        if (obj->getType() == ObjectType::Projectile) {
            projectilePool.release(std::static_pointer_cast<Projectile>(obj));
        } else if (obj->getType() == ObjectType::Bonus) {
            auto bonus = std::static_pointer_cast<ShieldBonus>(obj);
            if (bonus->isCollected()) {
                notify(GameEvent(EventType::ShieldCollected,
                                 static_cast<int>(Config::SHIELD_BONUS_DURATION), bonus->getPosition()));
            }
        }
    }

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                       [](const std::shared_ptr<GameObject>& o) { return !o->isActive(); }),
        objects.end());
}

void GameEngine::checkShipState(int healthBefore) {
    if (ship->getHealth() < healthBefore) {
        notify(GameEvent(EventType::ShipHit, ship->getHealth(), ship->getPosition()));
    }
    if (ship->getHealth() <= 0) {
        gameOver = true;
        notify(GameEvent(EventType::ShipDestroyed, 0, ship->getPosition()));
    }
}

void GameEngine::checkWaveCleared() {
    if (countAsteroids() > 0) {
        return;
    }

    notify(GameEvent(EventType::WaveCleared, wave));

    if (!endless && wave >= Config::FINAL_WAVE) {
        victory = true;
        notify(GameEvent(EventType::Victory, wave));
        return;
    }
    spawnWave(wave + 1);
}

int GameEngine::countAsteroids() const {
    return static_cast<int>(std::count_if(objects.begin(), objects.end(),
        [](const std::shared_ptr<GameObject>& o) {
            return o->getType() == ObjectType::Asteroid && o->isActive();
        }));
}


const std::vector<std::shared_ptr<GameObject>>& GameEngine::getObjects() const { return objects; }
std::shared_ptr<Spaceship> GameEngine::getShip() const { return ship; }
ProjectilePool& GameEngine::getProjectilePool() { return projectilePool; }
int GameEngine::getWave() const { return wave; }
bool GameEngine::isEndless() const { return endless; }
bool GameEngine::isGameOver() const { return gameOver; }
bool GameEngine::isVictory() const { return victory; }
