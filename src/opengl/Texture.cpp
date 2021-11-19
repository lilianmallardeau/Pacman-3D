//
// Created by lilian on 15/10/2021.
//

#include "Texture.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Renderer.h"


Texture::Texture(const std::string &filename) : filename(filename) {
    glGenTextures(1, &texture_id);
    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(true);
    unsigned char* buffer = stbi_load(filename.c_str(), &width, &height, &n_channels, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    if (buffer)
        stbi_image_free(buffer);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture_id);
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
