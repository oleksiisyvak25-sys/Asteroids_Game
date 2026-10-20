//
// Created by alex on 20.10.26.
//

#include "AsteroidFactory.h"
#include "Asteroid.h"
#include "NormalBehavior.h"
#include "ExplosiveBehavior.h"
#include "GravityBehavior.h"

std::shared_ptr<Asteroid> AsteroidFactory::create(const std::string& type, int size, Vector2 pos) {
    return create(type, size, pos, Vector2(0.0f, 0.0f));
}

std::shared_ptr<Asteroid> AsteroidFactory::create(const std::string& type, int size, Vector2 pos, Vector2 vel) {
    std::shared_ptr<AsteroidBehavior> behavior;

    if (type == "explosive") {
        behavior = std::make_shared<ExplosiveBehavior>();
    } else if (type == "gravity") {
        behavior = std::make_shared<GravityBehavior>();
    } else {
        behavior = std::make_shared<NormalBehavior>();
    }

    return std::make_shared<Asteroid>(size, pos, vel, behavior);
}