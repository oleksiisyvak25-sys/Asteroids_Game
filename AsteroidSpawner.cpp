#include "AsteroidSpawner.h"
#include "AsteroidFactory.h"
#include "Config.h"
#include <algorithm>

AsteroidSpawner::AsteroidSpawner(unsigned seed) : rng(seed) {}

float AsteroidSpawner::randomFloat(float lo, float hi) {
    std::uniform_real_distribution<float> dist(lo, hi);
    return dist(rng);
}

int AsteroidSpawner::randomInt(int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

int AsteroidSpawner::asteroidCountForWave(int waveNumber) {
    return std::min(2 + waveNumber, Config::MAX_WAVE_ASTEROIDS);
}

int AsteroidSpawner::specialChancePercent(int waveNumber) {
    return std::min(5 + 5 * waveNumber, 30);
}

AsteroidType AsteroidSpawner::rollType(int waveNumber) {
    const int chance = specialChancePercent(waveNumber);
    const int roll = randomInt(0, 99);
    if (roll < chance) {
        return AsteroidType::Explosive;
    }
    if (roll < chance * 2) {
        return AsteroidType::Gravity;
    }
    return AsteroidType::Normal;
}

std::vector<std::shared_ptr<Asteroid>> AsteroidSpawner::spawnWave(int waveNumber) {
    std::vector<std::shared_ptr<Asteroid>> waveAsteroids;

    const int count = asteroidCountForWave(waveNumber);
    const float minSpeed = std::min(25.0f + 4.0f * waveNumber, 100.0f);
    const float maxSpeed = std::min(55.0f + 6.0f * waveNumber, 140.0f);

    for (int i = 0; i < count; ++i) {
        Vector2 pos;
        Vector2 inward;

        switch (randomInt(0, 3)) {
            case 0:
                pos = Vector2(randomFloat(0.0f, Config::WORLD_WIDTH), 0.0f);
                inward = Vector2(0.0f, 1.0f);
                break;
            case 1:
                pos = Vector2(randomFloat(0.0f, Config::WORLD_WIDTH), Config::WORLD_HEIGHT);
                inward = Vector2(0.0f, -1.0f);
                break;
            case 2:
                pos = Vector2(0.0f, randomFloat(0.0f, Config::WORLD_HEIGHT));
                inward = Vector2(1.0f, 0.0f);
                break;
            default:
                pos = Vector2(Config::WORLD_WIDTH, randomFloat(0.0f, Config::WORLD_HEIGHT));
                inward = Vector2(-1.0f, 0.0f);
                break;
        }

        const Vector2 vel = inward.rotated(randomFloat(-60.0f, 60.0f)) * randomFloat(minSpeed, maxSpeed);
        waveAsteroids.push_back(AsteroidFactory::create(rollType(waveNumber), Config::ASTEROID_MAX_SIZE, pos, vel));
    }

    return waveAsteroids;
}
