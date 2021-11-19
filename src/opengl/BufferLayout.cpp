//
// Created by lilian on 03/10/2021.
//

#include "BufferLayout.h"

#include <iostream>
#include <glad/glad.h>


unsigned int GetTypeSize(GLenum type) {
    switch (type) {
        case GL_BYTE:           return sizeof(GLbyte);   // 8
        case GL_UNSIGNED_BYTE:  return sizeof(GLubyte);  // 8
        case GL_SHORT:          return sizeof(GLshort);  // 16
        case GL_UNSIGNED_SHORT: return sizeof(GLushort); // 16
        case GL_INT:            return sizeof(GLint);    // 32
        case GL_UNSIGNED_INT:   return sizeof(GLuint);   // 32
        case GL_FIXED:          return sizeof(GLfixed);  // 32
        case GL_HALF_FLOAT:     return sizeof(GLhalf);   // 16
        case GL_FLOAT:          return sizeof(GLfloat);  // 32
        case GL_DOUBLE:         return sizeof(GLdouble); // 64
        default: // Should not happen
            std::cerr << "Warning! Getting size of unsupported/unknown type! This shouldn't happen." << std::endl;
            return 0;
    }
}

unsigned int BufferLayout::getStride() const {
    unsigned int stride = 0;
    for (const VertexAttribute& attribute : vertexAttributes)
        stride += attribute.n_components * GetTypeSize(attribute.type);
    return stride;
}

const void* BufferLayout::getAttributeOffsetPointer(unsigned int attributeIndex) const {
    unsigned int offset = 0;
    for (unsigned int i = 0; i < attributeIndex; ++i)
        offset += vertexAttributes[i].n_components * GetTypeSize(vertexAttributes[i].type);
    return (const void*) offset;
}

unsigned int BufferLayout::attributesCount() const {
    return vertexAttributes.size();
}

const VertexAttribute &BufferLayout::operator[](unsigned int attribute) const {
    return vertexAttributes[attribute];
}

template <>
void BufferLayout::addAttribute<int>(unsigned int n_components, bool normalized) {
    vertexAttributes.push_back({GL_INT, n_components, normalized});
}

template <>
void BufferLayout::addAttribute<unsigned int>(unsigned int n_components, bool normalized) {
    vertexAttributes.push_back({GL_UNSIGNED_INT, n_components, normalized});
}

template <>
void BufferLayout::addAttribute<float>(unsigned int n_components, bool normalized) {
    vertexAttributes.push_back({GL_FLOAT, n_components, normalized});
}

template <>
void BufferLayout::addAttribute<double>(unsigned int n_components, bool normalized) {
    vertexAttributes.push_back({GL_DOUBLE, n_components, normalized});
}