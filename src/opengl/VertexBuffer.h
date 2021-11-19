//
// Created by lilian on 03/10/2021.
//

#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H


class VertexBuffer {
    private:
        unsigned int bufferId = 0;

    public:
        VertexBuffer();
        VertexBuffer(unsigned int size, const void* data);
        VertexBuffer(const VertexBuffer &) = delete;
        ~VertexBuffer();

        void setData(unsigned int size, const void* data);

        void Bind() const;
        static void Unbind();
};


#endif //VERTEX_BUFFER_H
