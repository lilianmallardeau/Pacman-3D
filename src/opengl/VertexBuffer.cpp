//
// Created by lilian on 03/10/2021.
//

#include "VertexBuffer.h"

#include <glad/glad.h>


VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &bufferId);
}

VertexBuffer::VertexBuffer(unsigned int size, const void *data) : VertexBuffer() {
    setData(size, data);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &bufferId);
}

//NOLINTNEXTLINE(readability-make-member-function-const)
void VertexBuffer::setData(unsigned int size, const void *data) {
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
}

void VertexBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
