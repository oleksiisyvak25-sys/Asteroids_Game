#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(std::size_t initialSize) {
    for (std::size_t i = 0; i < initialSize; ++i) {
        pool.push_back(std::make_shared<Projectile>());
    }
}

std::shared_ptr<Projectile> ProjectilePool::acquire() {
    for (auto& p : pool) {
        if (!p->isActive()) {
            p->reset();
            return p;
        }
    }

    auto extra = std::make_shared<Projectile>();
    extra->reset();
    pool.push_back(extra);
    return extra;
}

void ProjectilePool::release(const std::shared_ptr<Projectile>& projectile) {
    if (projectile) {
        projectile->deactivate();
    }
}

void ProjectilePool::releaseAll() {
    for (auto& p : pool) {
        p->deactivate();
    }
}

std::size_t ProjectilePool::size() const {
    return pool.size();
}

std::size_t ProjectilePool::activeCount() const {
    std::size_t count = 0;
    for (const auto& p : pool) {
        if (p->isActive()) {
            ++count;
        }
    }
    return count;
}
