#include "CollisionSystem.h"

void CollisionSystem::check(const std::vector<std::shared_ptr<GameObject>>& objects) {
    for (std::size_t i = 0; i < objects.size(); ++i) {
        for (std::size_t j = i + 1; j < objects.size(); ++j) {
            GameObject& a = *objects[i];
            GameObject& b = *objects[j];

            if (!a.isActive() || !b.isActive()) {
                continue;
            }

            const float dist = a.getPosition().distanceTo(b.getPosition());
            if (dist <= a.getRadius() + b.getRadius()) {
                a.onCollision(b);
                b.onCollision(a);
            }
        }
    }
}
