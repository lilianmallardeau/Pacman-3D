//
// Created by lilian on 03/10/2021.
//

#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include <vector>
#include <glad/glad.h>


struct VertexAttribute {
    GLenum type;
    unsigned int n_components;
    bool normalized;
};

class BufferLayout {
    private:
        std::vector<VertexAttribute> vertexAttributes;

    public:
        [[nodiscard]] unsigned int getStride() const;
        [[nodiscard]] const void* getAttributeOffsetPointer(unsigned int attributeIndex) const;

        [[nodiscard]] unsigned int attributesCount() const;
        const VertexAttribute& operator[](unsigned int attribute) const;

        template<typename T>
        void addAttribute(unsigned int n_components, bool normalized = false);
};


#endif //BUFFER_LAYOUT_H
