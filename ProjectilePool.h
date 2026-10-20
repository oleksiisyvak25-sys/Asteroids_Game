//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_PROJECTILEPOOL_H
#define ASTEROIDS_GAME_PROJECTILEPOOL_H

#include "Projectile.h"
#include <vector>
#include <memory>

class ProjectilePool {
private:
    std::vector<std::shared_ptr<Projectile>> pool;

public:
    ProjectilePool(size_t initialSize = 30);

    std::shared_ptr<Projectile> acquire(Vector2 pos, Vector2 vel);

    void release(const std::shared_ptr<Projectile>& projectile);
};


#endif //ASTEROIDS_GAME_PROJECTILEPOOL_H
