//
// Created by lilian on 04/10/2021.
//

#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

#include <glm/glm.hpp>

#include "../opengl/opengl_abstraction.hpp"


enum class Direction {
    LEFT, UP, RIGHT, DOWN,
    NONE
};


template<typename T>
struct PositionTemplate {
    T x, y;
    PositionTemplate<T>() : x(T()), y(T()) {};
    PositionTemplate<T>(T x, T y) : x(x), y(y) {};
    PositionTemplate<T>(const glm::vec2& v) : x(v.x), y(v.y) {};
    PositionTemplate<T> operator+(const PositionTemplate & pos) const {
        return {x + pos.x, y + pos.y};
    }
    PositionTemplate<T>& operator+=(const PositionTemplate & pos) {
        x += pos.x;
        y += pos.y;
        return *this;
    }
    PositionTemplate<T>& operator+=(T diff) {
        return *this += {diff, diff};
    }
    float operator%(const PositionTemplate & pos) const {
        return sqrtf((x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y));
    }
    [[nodiscard]] PositionTemplate<T> round() const {
        return {std::round(x), std::round(y)};
    }
    [[nodiscard]] PositionTemplate<T> move(Direction dir, T distance = 1) const {
        switch (dir) {
            case Direction::LEFT:   return {x-distance, y};
            case Direction::RIGHT:  return {x+distance, y};
            case Direction::UP:     return {x, y+distance};
            case Direction::DOWN:   return {x, y-distance};
            default: return *this;
        }
    }
    PositionTemplate<T>& go(Direction dir, T distance = 1) {
        *this = move(dir, distance);
        if (isCentered()) *this = round();
        return *this;
    }
    [[nodiscard]] bool isHalfSquareWay(Direction dir) const {
        switch (dir) {
            case Direction::LEFT: return round().x >= x;
            case Direction::RIGHT: return round().x <= x;
            case Direction::UP: return round().y <= y;
            case Direction::DOWN: return round().y >= y;
            default: return true;
        }
    }
    [[nodiscard]] bool isCentered() const {
        return *this % round() < 1e-4;
    }
    operator PositionTemplate<float>() const { return {(float) x, (float) y}; }; //NOLINT(google-explicit-constructor)
    operator PositionTemplate<unsigned int>() const { return {(unsigned int) std::round(x), (unsigned int) std::round(y)}; }; //NOLINT(google-explicit-constructor)
    operator glm::vec2() const { return {x, y}; }; //NOLINT(google-explicit-constructor)
};

using Position = PositionTemplate<float>;
using MapPosition = PositionTemplate<unsigned int>;


// Note: comparing two Position (aka PositionTemplate<float>) will result in casting
//  them both into MapPosition and them comparing the result
// The comparison will return true if the 2 positions are in the same map square
inline bool operator==(const MapPosition& pos1, const MapPosition& pos2) {
    return pos1.x == pos2.x && pos1.y == pos2.y;
}
// Uncomment the next operator to disable the behaviour described in the previous comment
//inline bool operator==(const Position& pos1, const Position& pos2) {
//    return pos1 % pos2 < 1e-4;
//}


enum class MapElement {
    UNDEFINED,
    PELLET = '0',
    WALL   = '1',
    TUNNEL = '2'
};


class Map {
    private:
        unsigned int width, height;
        std::vector<std::vector<MapElement>> map;
        MapPosition initialPosition;

        // Vertices and indexes
        std::vector<float> vertices;
        std::vector<unsigned int> wallsIndices;
        std::vector<unsigned int> groundIndices;
        friend class Game; // No need for getters yeyyy

        // OpenGL
        VertexBuffer *vertexBuffer;
        VertexArray* vertexArray;
        IndexBuffer *wallsIndexBuffer, *groundIndexBuffer;
        Program *shader;
        Texture *wallTexture;

        // Methods
        void loadMap(const std::string & filename);
        void computeVertices();
        void computeWallIndexes();
        void computeGroundIndexes();

    public:
        explicit Map(const std::string & filename);
        void init();

        bool removePellet(const MapPosition & position);
        [[nodiscard]] bool canGo(const MapPosition & position, Direction direction) const;
        [[nodiscard]] bool canGo(const Position & position, Direction currentDirection, Direction newDirection) const;
        [[nodiscard]] bool canTurn(const MapPosition & position, Direction currentDirection) const;
        [[nodiscard]] bool canTurn(const Position & position, Direction currentDirection) const;

        // Getters, Setters
        [[nodiscard]] unsigned int getWidth() const { return width; };
        [[nodiscard]] unsigned int getHeight() const { return height; };
        [[nodiscard]] unsigned int count(MapElement) const;
        [[nodiscard]] unsigned int pelletsCount() const;
        [[nodiscard]] const MapPosition & getInitialPosition() const { return initialPosition; };

        [[nodiscard]] MapPosition getRandomPosition(MapElement type) const;
        [[nodiscard]] Direction getRandomDirection(const MapPosition & pos) const;
        [[nodiscard]] static Direction getOppositeDirection(Direction dir);
        [[nodiscard]] bool isTunnel(MapPosition pos) const;

        // Operators
        MapElement operator[] (const MapPosition & position) const;
};


#endif //MAP_H
