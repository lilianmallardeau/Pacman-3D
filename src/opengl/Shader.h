//
// Created by lilian on 03/10/2021.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>


enum class ShaderType {
    COMPUTE_SHADER = GL_COMPUTE_SHADER,
    VERTEX_SHADER = GL_VERTEX_SHADER,
    TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

// A Shader object must always be sent by reference! Otherwise, because of the destructor,
// it could result in the linked shader object from the GPU to be deleted
// Edit: nvm, I deleted the copy constructor
class Shader {
    private:
        unsigned int shaderId;
        ShaderType type;
        bool Compile();

    public:
        Shader(const std::string & sourceFile, ShaderType shaderType);
        Shader(const Shader &) = delete;
        ~Shader();

        [[nodiscard]] inline unsigned int getId() const { return shaderId; };
        operator unsigned int() const { return shaderId; };
};


#endif //SHADER_H
