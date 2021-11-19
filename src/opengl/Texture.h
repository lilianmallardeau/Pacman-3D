//
// Created by lilian on 15/10/2021.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>


class Texture {
    private:
        std::string filename;
        unsigned int texture_id = 0;
        int width = 0, height = 0, n_channels = 0;

    public:
        explicit Texture(const std::string & filename);
        Texture(const Texture &) = delete;
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        static void Unbind();

        [[nodiscard]] unsigned int getWidth() const { return width; }
        [[nodiscard]] unsigned int getHeight() const { return height; }
        [[nodiscard]] unsigned int getNChannels() const { return n_channels; }
};


#endif //TEXTURE_H
