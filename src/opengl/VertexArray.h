//
// Created by lilian on 03/10/2021.
//

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "BufferLayout.h"


class VertexArray {
    private:
        unsigned int vao_id = 0;
        unsigned int attributes_count = 0;

    public:
        VertexArray();
        VertexArray(const VertexArray &) = delete;
        ~VertexArray();

        void addBuffer(const VertexBuffer& buffer, const BufferLayout& layout);
        void addAttribute(const VertexBuffer& buffer, const BufferLayout& layout, unsigned int attribute);

        void EnableAttribute(unsigned int attribute) const;
        void DisableAttribute(unsigned int attribute) const;

        void Bind() const;
        static void Unbind();
};


#endif //VERTEX_ARRAY_H
