//
// Created by lilian on 03/10/2021.
//

#include "IndexBuffer.h"

#include <glad/glad.h>


IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &id);
}

IndexBuffer::IndexBuffer(unsigned int length, const unsigned int *data) : IndexBuffer() {
    setData(length, data);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id);
}

void IndexBuffer::setData(unsigned int length, const unsigned int *data) {
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, length*sizeof(unsigned int), data, GL_STATIC_DRAW);
    this->length = length;
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
