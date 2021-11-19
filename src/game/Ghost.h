//
// Created by lilian on 17/11/2021.
//

#ifndef GHOST_H
#define GHOST_H

#include <glm/glm.hpp>

#include "../utils/Model.h"
#include "../utils/Object3D.h"
#include "Map.h"


class Ghost {
    private:
        Object3D obj;
        Position position;
        Direction direction;
        friend class Game;

    public:
        Ghost(Position pos, Model* model) : obj(model), position(pos) {};
        void updateObjectPosition();

        [[nodiscard]] Position getPosition() const { return position; };
        [[nodiscard]] Direction getDirection() const { return direction; };
        void setPosition(const Position& pos) { position = pos; };
        void setDirection(Direction dir);

        void move(const Position & diff);
        void move(float distance = 0.05f);

};


#endif //GHOST_H
