#ifndef ASTEROIDS_GAME_GAMEENGINE_H
#define ASTEROIDS_GAME_GAMEENGINE_H

#include "GameObject.h"
#include "GameObserver.h"
#include "CollisionSystem.h"
#include "AsteroidSpawner.h"
#include "ProjectilePool.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include <vector>
#include <memory>
#include <random>

class GameEngine {
private:
    std::vector<std::shared_ptr<GameObject>> objects;
    std::vector<std::shared_ptr<GameObserver>> observers;
    CollisionSystem collisionSystem;
    AsteroidSpawner asteroidSpawner;
    ProjectilePool projectilePool;
    std::shared_ptr<Spaceship> ship;
    std::mt19937 rng;
    int wave;
    bool endless;
    bool gameOver;
    bool victory;

    void step(float dt);
    void collectDestroyedAsteroids(std::vector<std::shared_ptr<Asteroid>>& out);
    void processDestroyedAsteroids();
    void explode(const Asteroid& source);
    void removeInactiveObjects();
    void checkShipState(int healthBefore);
    void checkWaveCleared();
    int countAsteroids() const;

public:
    explicit GameEngine(unsigned seed = std::random_device{}());

    void reset();
    void update(float dt);

    void addObject(const std::shared_ptr<GameObject>& o);
    void removeObject(const std::shared_ptr<GameObject>& o);
    void subscribe(const std::shared_ptr<GameObserver>& o);
    void notify(const GameEvent& e);

    void spawnWave(int waveNumber);
    void enableEndlessMode();

    const std::vector<std::shared_ptr<GameObject>>& getObjects() const;
    std::shared_ptr<Spaceship> getShip() const;
    ProjectilePool& getProjectilePool();
    int getWave() const;
    bool isEndless() const;
    bool isGameOver() const;
    bool isVictory() const;
};

#endif //ASTEROIDS_GAME_GAMEENGINE_H
