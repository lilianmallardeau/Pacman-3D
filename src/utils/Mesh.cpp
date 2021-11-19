//
// Created by lilian on 17/11/2021.
//

#include "Mesh.h"

void Mesh::setupBuffers() const {
    vbo = new VertexBuffer();
    ibo = new IndexBuffer();
    vao = new VertexArray();

    vbo->setData(sizeof(Vertex) * vertices.size(), vertices.data());

    BufferLayout layout;
    layout.addAttribute<float>(3);
    layout.addAttribute<float>(3);
    layout.addAttribute<float>(2);
    vao->addBuffer(*vbo, layout);

    ibo->setData(indices.size(), indices.data());
}

Mesh::~Mesh() {
    delete vbo;
    delete ibo;
    delete vao;
}
