//
// Created by alex on 20.10.26.
//

#include "GameObject.h"

GameObject::GameObject()
    : position(0.0f, 0.0f),
      velocity(0.0f, 0.0f),
      angle(0.0f),
      radius(10.0f),
      isActive(true) {}

void GameObject::update(float dt) {
    position = position + velocity * dt;
    wrapAround(800.0f, 600.0f);
}

void GameObject::wrapAround(float screenWidth, float screenHeight) {
    if (position.x < 0.0f) position.x += screenWidth;
    if (position.x > screenWidth) position.x -= screenWidth;
    if (position.y < 0.0f) position.y += screenHeight;
    if (position.y > screenHeight) position.y -= screenHeight;
}