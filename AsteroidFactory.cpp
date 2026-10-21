#include "AsteroidFactory.h"
#include "Asteroid.h"
#include "NormalBehavior.h"
#include "ExplosiveBehavior.h"
#include "GravityBehavior.h"

std::shared_ptr<Asteroid> AsteroidFactory::create(AsteroidType type, int size, Vector2 pos) {
    return create(type, size, pos, Vector2(0.0f, 0.0f));
}

std::shared_ptr<Asteroid> AsteroidFactory::create(AsteroidType type, int size, Vector2 pos, Vector2 vel) {
    std::shared_ptr<AsteroidBehavior> behavior;

    switch (type) {
        case AsteroidType::Explosive:
            behavior = std::make_shared<ExplosiveBehavior>();
            break;
        case AsteroidType::Gravity:
            behavior = std::make_shared<GravityBehavior>();
            break;
        case AsteroidType::Normal:
        default:
            behavior = std::make_shared<NormalBehavior>();
            break;
    }

    return std::make_shared<Asteroid>(type, size, pos, vel, behavior);
}
