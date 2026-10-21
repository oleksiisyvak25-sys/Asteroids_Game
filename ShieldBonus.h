#ifndef ASTEROIDS_GAME_SHIELDBONUS_H
#define ASTEROIDS_GAME_SHIELDBONUS_H

#include "GameObject.h"

class ShieldBonus : public GameObject {
private:
    float lifetime;
    bool collected;

public:
    ShieldBonus(Vector2 pos, Vector2 vel);
    ~ShieldBonus() override = default;

    ObjectType getType() const override;
    void update(float dt) override;
    void onCollision(GameObject& other) override;

    bool isCollected() const;
};

#endif //ASTEROIDS_GAME_SHIELDBONUS_H
