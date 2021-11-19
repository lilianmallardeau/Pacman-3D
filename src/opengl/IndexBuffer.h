//
// Created by lilian on 03/10/2021.
//

#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H


class IndexBuffer {
    private:
        unsigned int id = 0;
        unsigned int length = 0;

    public:
        IndexBuffer();
        IndexBuffer(unsigned int length, const unsigned int *data);
        IndexBuffer(const IndexBuffer &) = delete;
        ~IndexBuffer();

        void setData(unsigned int length, const unsigned int *data);

        void Bind() const;
        static void Unbind();

        [[nodiscard]] inline unsigned int getLength() const { return length; };
};


#endif //INDEX_BUFFER_H
