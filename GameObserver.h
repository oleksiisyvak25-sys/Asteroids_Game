#ifndef ASTEROIDS_GAME_GAMEOBSERVER_H
#define ASTEROIDS_GAME_GAMEOBSERVER_H

#include "Vector2.h"

enum class EventType {
    GameStarted,
    WaveStarted,
    WaveCleared,
    ProjectileFired,
    AsteroidDestroyed,
    Explosion,
    BonusDropped,
    ShieldCollected,
    ShipHit,
    ShipDestroyed,
    Victory
};

struct GameEvent {
    EventType type;
    int value;
    Vector2 position;

    GameEvent(EventType t, int val = 0, Vector2 pos = Vector2(0.0f, 0.0f));
};

class GameObserver {
public:
    virtual ~GameObserver() = default;
    virtual void onEvent(const GameEvent& event) = 0;
};

#endif //ASTEROIDS_GAME_GAMEOBSERVER_H
