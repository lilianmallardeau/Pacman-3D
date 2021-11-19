//
// Created by lilian on 17/11/2021.
//

#ifndef MESH_H
#define MESH_H

#include <utility>
#include <vector>
#include <glm/glm.hpp>

#include "../opengl/opengl_abstraction.hpp"


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

    public:
        mutable VertexBuffer *vbo{};
        mutable IndexBuffer *ibo{};
        mutable VertexArray *vao{};

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(std::move(vertices)), indices(std::move(indices)) {};
        ~Mesh();
        void setupBuffers() const;
};


#endif //MESH_H
