#ifndef ASTEROIDS_GAME_CONFIG_H
#define ASTEROIDS_GAME_CONFIG_H

namespace Config {
    constexpr float WORLD_WIDTH  = 800.0f;
    constexpr float WORLD_HEIGHT = 600.0f;
    constexpr float PI           = 3.14159265358979f;

    constexpr float MAX_PHYSICS_STEP = 1.0f / 60.0f;
    constexpr float MAX_FRAME_TIME   = 0.1f;
    constexpr float TURN_TIME        = 0.25f;

    constexpr int   SHIP_START_HEALTH      = 3;
    constexpr float SHIP_RADIUS            = 16.0f;
    constexpr float SHIP_ROTATION_STEP     = 15.0f;
    constexpr float SHIP_THRUST_IMPULSE    = 30.0f;
    constexpr float SHIP_MAX_SPEED         = 220.0f;
    constexpr float SHIP_SHOOT_COOLDOWN    = 0.2f;
    constexpr float SHIP_HIT_INVULNERABILITY = 2.0f;
    constexpr float SHIELD_BONUS_DURATION  = 5.0f;

    constexpr float PROJECTILE_SPEED    = 350.0f;
    constexpr float PROJECTILE_LIFETIME = 2.0f;
    constexpr float PROJECTILE_RADIUS   = 3.0f;
    constexpr int   PROJECTILE_POOL_SIZE = 20;

    constexpr int   ASTEROID_MAX_SIZE        = 3;
    constexpr int   ASTEROID_MIN_SIZE        = 1;
    constexpr float ASTEROID_RADIUS_PER_SIZE = 14.0f;
    constexpr int   ASTEROID_DAMAGE          = 1;
    constexpr float FRAGMENT_MIN_SPEED       = 25.0f;
    constexpr float FRAGMENT_SPREAD          = 35.0f;

    constexpr float BLAST_BASE_RADIUS     = 40.0f;
    constexpr float BLAST_RADIUS_PER_SIZE = 25.0f;
    constexpr int   BLAST_DAMAGE          = 1;

    constexpr float GRAVITY_RANGE    = 250.0f;
    constexpr float GRAVITY_STRENGTH = 100.0f;

    constexpr int FINAL_WAVE      = 5;
    constexpr int MAX_WAVE_ASTEROIDS = 12;

    constexpr float BONUS_DROP_CHANCE = 0.15f;
    constexpr float BONUS_LIFETIME    = 8.0f;
    constexpr float BONUS_RADIUS      = 10.0f;
}

#endif //ASTEROIDS_GAME_CONFIG_H
