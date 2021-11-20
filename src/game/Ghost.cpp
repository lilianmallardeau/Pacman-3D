//
// Created by lilian on 17/11/2021.
//

#include "Ghost.h"


void Ghost::updateObjectPosition() {
    obj.setTranslation(glm::vec3(position.x, position.y, 0));
}

void Ghost::setDirection(Direction dir) {
    direction = dir;
    switch (dir) {
        case Direction::RIGHT: obj.setRotation(0, 0, 0); break;
        case Direction::UP: obj.setRotation(0, 0, 90); break;
        case Direction::LEFT: obj.setRotation(0, 0, 180); break;
        case Direction::DOWN: obj.setRotation(0, 0, 270); break;
        default: break;
    }
}

void Ghost::move(const Position &diff) {
    setPosition(position + diff);
    updateObjectPosition();
}

void Ghost::move(float distance) {
    position.go(direction, distance);
    updateObjectPosition();
}
