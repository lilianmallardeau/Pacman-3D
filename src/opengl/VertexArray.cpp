//
// Created by lilian on 03/10/2021.
//

#include "VertexArray.h"

#include <glad/glad.h>


VertexArray::VertexArray() {
    glCreateVertexArrays(1, &vao_id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &vao_id);
}

void VertexArray::addBuffer(const VertexBuffer& buffer, const BufferLayout& layout) {
    Bind();
    buffer.Bind();
    for (unsigned int i = 0; i < layout.attributesCount(); ++i) {
        glVertexAttribPointer(attributes_count, (int) layout[i].n_components, layout[i].type, layout[i].normalized ? GL_TRUE : GL_FALSE, (int) layout.getStride(), layout.getAttributeOffsetPointer(i));
        EnableAttribute(attributes_count);
        ++attributes_count;
    }
}

void VertexArray::addAttribute(const VertexBuffer& buffer, const BufferLayout& layout, unsigned int attribute) {
    Bind();
    buffer.Bind();
    glVertexAttribPointer(attributes_count, (int) layout[attribute].n_components, layout[attribute].type, layout[attribute].normalized ? GL_TRUE : GL_FALSE, (int) layout.getStride(), layout.getAttributeOffsetPointer(attribute));
    EnableAttribute(attributes_count);
    ++attributes_count;
}

void VertexArray::EnableAttribute(unsigned int attribute) const {
    glEnableVertexArrayAttrib(vao_id, attribute);
}

void VertexArray::DisableAttribute(unsigned int attribute) const {
    glDisableVertexArrayAttrib(vao_id, attribute);
}

void VertexArray::Bind() const {
    glBindVertexArray(vao_id);
}

void VertexArray::Unbind() {
    glBindVertexArray(0);
}
