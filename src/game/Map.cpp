//
// Created by lilian on 04/10/2021.
//

#include "Map.h"

#include <iostream>
#include <fstream>

#include "../parameters.h"


Map::Map(const std::string &filename) {
    loadMap(filename);
}

void Map::init() {
    vertexArray = new VertexArray();
    vertexBuffer = new VertexBuffer();
    wallsIndexBuffer = new IndexBuffer();
    groundIndexBuffer = new IndexBuffer();

    shader = new Program("src/shaders/map/vertex.shader", "src/shaders/map/fragment.shader");
    wallTexture = new Texture(WALLS_TEXTURE);
    wallTexture->Bind(0);
    shader->setUniform<int>("u_Texture", {0});

    computeVertices();
    vertexBuffer->setData(vertices.size()*sizeof(float), vertices.data());
    BufferLayout layout;
    layout.addAttribute<float>(3);
    layout.addAttribute<float>(2);
    vertexArray->addBuffer(*vertexBuffer, layout);

    computeWallIndexes();
    computeGroundIndexes();
    wallsIndexBuffer->setData(wallsIndices.size(), wallsIndices.data());
    groundIndexBuffer->setData(groundIndices.size(), groundIndices.data());
}

void Map::loadMap(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string firstline;
    std::getline(file, firstline);
    sscanf(firstline.c_str(), "%ux%u", &width, &height); //NOLINT(cert-err34-c)
    map.resize(width, std::vector<MapElement>(height, MapElement::UNDEFINED));
    char c;
    MapPosition pos{0, height-1};
    while (file.get(c)) {
        if (c == '\n') {
            pos = {0, pos.y-1};
            continue;
        }
        if ('0' <= c && c <= '2') {
            map[pos.x][pos.y] = static_cast<MapElement>(c);
            if (c == '2')
                initialPosition = pos;
            ++pos.x;
        }
    }
}

void Map::computeVertices() {
    vertices.clear();
    vertices.reserve(2 * 3 * (width+1) * (height+1));
    for (int u = 0; u <= 1; ++u) {
        for (int z = 0; z <= 1; ++z) {
            for (unsigned int y = 0; y <= height; ++y) {
                for (unsigned int x = 0; x <= width; ++x) {
                    vertices.push_back((float) x - .5f);
                    vertices.push_back((float) y - .5f);
                    vertices.push_back((float) z);
                    vertices.push_back((float) u); // u text coord
                    vertices.push_back((float) z); // v text coord
                    // todo: add normals?
                }
            }
        }
    }
}

void Map::computeWallIndexes() {
    wallsIndices.clear();
    unsigned int z_offset = (width+1) * (height+1);
    unsigned int texture_offset = 2 * z_offset;
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            if (isTunnel({x, y})) {
                if (!isTunnel({x+1, y}) || x+1 == width) {
                    wallsIndices.push_back(texture_offset + (width+1)*y + x+1);
                    wallsIndices.push_back((width+1)*(y+1) + x+1);
                    wallsIndices.push_back(z_offset + (width+1)*(y+1) + x+1);
                    wallsIndices.push_back(z_offset + (width+1)*(y+1) + x+1);
                    wallsIndices.push_back(texture_offset + z_offset + (width+1)*y + x+1);
                    wallsIndices.push_back(texture_offset + (width+1)*y + x+1);
                }
                if (!isTunnel({x, y+1}) || y+1 == height) {
                    wallsIndices.push_back(texture_offset + (width+1)*(y+1) + x+1);
                    wallsIndices.push_back((width+1)*(y+1) + x);
                    wallsIndices.push_back(z_offset + (width+1)*(y+1) + x);
                    wallsIndices.push_back(z_offset + (width+1)*(y+1) + x);
                    wallsIndices.push_back(texture_offset + z_offset + (width+1)*(y+1) + x+1);
                    wallsIndices.push_back(texture_offset + (width+1)*(y+1) + x+1);
                }
                if (!isTunnel({x-1, y}) || x == 0) {
                    wallsIndices.push_back(texture_offset + (width+1)*(y+1) + x);
                    wallsIndices.push_back((width+1)*y + x);
                    wallsIndices.push_back(z_offset + (width+1)*y + x);
                    wallsIndices.push_back(z_offset + (width+1)*y + x);
                    wallsIndices.push_back(texture_offset + z_offset + (width+1)*(y+1) + x);
                    wallsIndices.push_back(texture_offset + (width+1)*(y+1) + x);
                }
                if (!isTunnel({x, y-1}) || y == 0) {
                    wallsIndices.push_back(texture_offset + (width+1)*y + x);
                    wallsIndices.push_back((width+1)*y + x+1);
                    wallsIndices.push_back(z_offset + (width+1)*y + x+1);
                    wallsIndices.push_back(z_offset + (width+1)*y + x+1);
                    wallsIndices.push_back(texture_offset + z_offset + (width+1)*y + x);
                    wallsIndices.push_back(texture_offset + (width+1)*y + x);
                }
            }
        }
    }
}

void Map::computeGroundIndexes() {
    groundIndices.clear();
//    unsigned int texture_offset = 2 * (width+1) * (height+1);
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            if (isTunnel({x, y})) {
                groundIndices.push_back((width+1)*y + x);
                groundIndices.push_back((width+1)*(y+1) + x);
                groundIndices.push_back((width+1)*(y+1) + x+1);
                groundIndices.push_back((width+1)*(y+1) + x+1);
                groundIndices.push_back((width+1)*y + x+1);
                groundIndices.push_back((width+1)*y + x);
            }
        }
    }
}

bool Map::removePellet(const MapPosition &position) {
    if ((*this)[position] == MapElement::PELLET) {
        map[position.x][position.y] = MapElement::TUNNEL;
        return true;
    }
    return false;
}

bool Map::canGo(const MapPosition & position, Direction direction) const {
    MapElement next = (*this)[position.move(direction)];
    return next == MapElement::TUNNEL || next == MapElement::PELLET;
}

bool Map::canGo(const Position & position, Direction currentDirection, Direction newDirection) const {
    if (newDirection != currentDirection && newDirection != Map::getOppositeDirection(currentDirection))
        return position.isCentered() && canGo(MapPosition(position), newDirection);
    else if (newDirection == currentDirection) {
        if (position.isCentered())
            return canGo(MapPosition(position), newDirection);
        else
            return !position.isHalfSquareWay(currentDirection) || canGo(MapPosition(position), newDirection);
    }
    else return newDirection == Map::getOppositeDirection(currentDirection);
}

bool Map::canTurn(const MapPosition &position, Direction currentDirection) const {
    if (canGo(position, Direction::UP) && Direction::UP != currentDirection && Direction::UP != getOppositeDirection(currentDirection)) return true;
    if (canGo(position, Direction::DOWN) && Direction::DOWN != currentDirection && Direction::DOWN != getOppositeDirection(currentDirection)) return true;
    if (canGo(position, Direction::LEFT) && Direction::LEFT != currentDirection && Direction::LEFT != getOppositeDirection(currentDirection)) return true;
    if (canGo(position, Direction::RIGHT) && Direction::RIGHT != currentDirection && Direction::RIGHT != getOppositeDirection(currentDirection)) return true;
    return false;
}

bool Map::canTurn(const Position &position, Direction currentDirection) const {
    return position.isCentered() && canTurn(MapPosition(position), currentDirection);
}

unsigned int Map::count(MapElement element) const {
    unsigned int count = 0;
    for (unsigned int x = 0; x < width; ++x)
        for (unsigned int y = 0; y < height; ++y)
            if ((*this)[{x, y}] == element)
                ++count;
    return count;
}

unsigned int Map::pelletsCount() const {
    return count(MapElement::PELLET);
}

MapPosition Map::getRandomPosition(MapElement type) const {
    if (!count(type))
        throw std::runtime_error("Trying to get random position of non-present map element");
    MapPosition pos;
    do {
        pos = {rand() % width, rand() % height}; //NOLINT(cert-msc50-cpp)
    } while ((*this)[pos] != type);
    return pos;
}

Direction Map::getRandomDirection(const MapPosition &pos) const {
    Direction direction;
    do {
        direction = static_cast<Direction>(rand() % 4); //NOLINT(cert-msc50-cpp)
    } while (!canGo(pos, direction));
    return direction;
}

Direction Map::getOppositeDirection(Direction dir) {
    return static_cast<Direction>(((int) dir + 2) % 4);
}

MapElement Map::operator[](const MapPosition & position) const {
    if (position.x < width && position.y < height)
        return map[position.x][position.y];
    return MapElement::UNDEFINED;
}

bool Map::isTunnel(MapPosition pos) const {
    return (*this)[pos] == MapElement::TUNNEL || (*this)[pos] == MapElement::PELLET;
}


template<> MapPosition::operator glm::vec2() const {
    return glm::vec2(Position(*this));
}