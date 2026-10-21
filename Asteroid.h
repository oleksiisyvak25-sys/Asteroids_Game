#ifndef ASTEROIDS_GAME_ASTEROID_H
#define ASTEROIDS_GAME_ASTEROID_H

#include "GameObject.h"
#include "AsteroidBehavior.h"
#include "AsteroidType.h"
#include <memory>
#include <vector>

class Spaceship;

class Asteroid : public GameObject {
private:
    int size;
    AsteroidType kind;
    std::shared_ptr<AsteroidBehavior> behavior;
    float blastRadius;

public:
    Asteroid(AsteroidType asteroidKind, int asteroidSize, Vector2 pos, Vector2 vel,
             std::shared_ptr<AsteroidBehavior> asteroidBehavior);
    ~Asteroid() override = default;

    ObjectType getType() const override;
    void onCollision(GameObject& other) override;

    void destroy();
    void applyEffect(Spaceship& ship, float dt);
    std::vector<std::shared_ptr<Asteroid>> split() const;

    int getSize() const;
    AsteroidType getKind() const;
    float getBlastRadius() const;
    void setBlastRadius(float r);
};

#endif //ASTEROIDS_GAME_ASTEROID_H
