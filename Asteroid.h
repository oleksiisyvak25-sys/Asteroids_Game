//
// Created by alex on 20.10.26.
//

#ifndef ASTEROIDS_GAME_ASTEROID_H
#define ASTEROIDS_GAME_ASTEROID_H

#include "GameObject.h"
#include "AsteroidBehavior.h"
#include <memory>
#include <vector>

class Asteroid : public GameObject {
public:
    int size;
    std::shared_ptr<AsteroidBehavior> behavior;

    Asteroid(int size, Vector2 pos, Vector2 vel, std::shared_ptr<AsteroidBehavior> behavior);
    ~Asteroid() override = default;

    ObjectType getType() const override;
    void onCollision(GameObject* other) override;

    std::vector<std::shared_ptr<Asteroid>> split();
};


#endif //ASTEROIDS_GAME_ASTEROID_H
