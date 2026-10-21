#include "GameObserver.h"

GameEvent::GameEvent(EventType t, int val, Vector2 pos)
    : type(t), value(val), position(pos) {}
