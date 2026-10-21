#ifndef ASTEROIDS_GAME_COLLISIONSYSTEM_H
#define ASTEROIDS_GAME_COLLISIONSYSTEM_H

#include "GameObject.h"
#include <vector>
#include <memory>

class CollisionSystem {
public:
    CollisionSystem() = default;

    void check(const std::vector<std::shared_ptr<GameObject>>& objects);
};

#endif //ASTEROIDS_GAME_COLLISIONSYSTEM_H
