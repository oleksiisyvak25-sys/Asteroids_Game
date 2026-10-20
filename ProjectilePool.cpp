//
// Created by alex on 20.10.26.
//

#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(size_t initialSize) {
    for (size_t i = 0; i < initialSize; ++i) {
        pool.push_back(std::make_shared<Projectile>());
    }
}

std::shared_ptr<Projectile> ProjectilePool::acquire(Vector2 pos, Vector2 vel) {
    for (auto& p : pool) {
        if (!p->isActive) {
            p->init(pos, vel);
            return p;
        }
    }

    auto newProj = std::make_shared<Projectile>();
    newProj->init(pos, vel);
    pool.push_back(newProj);
    return newProj;
}

void ProjectilePool::release(const std::shared_ptr<Projectile>& projectile) {
    if (projectile) {
        projectile->isActive = false;
    }
}
