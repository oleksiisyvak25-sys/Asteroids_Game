#ifndef ASTEROIDS_GAME_PROJECTILEPOOL_H
#define ASTEROIDS_GAME_PROJECTILEPOOL_H

#include "Projectile.h"
#include "Config.h"
#include <vector>
#include <memory>
#include <cstddef>

class ProjectilePool {
private:
    std::vector<std::shared_ptr<Projectile>> pool;

public:
    explicit ProjectilePool(std::size_t initialSize = Config::PROJECTILE_POOL_SIZE);

    std::shared_ptr<Projectile> acquire();
    void release(const std::shared_ptr<Projectile>& projectile);
    void releaseAll();

    std::size_t size() const;
    std::size_t activeCount() const;
};

#endif //ASTEROIDS_GAME_PROJECTILEPOOL_H
