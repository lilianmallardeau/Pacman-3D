//
// Created by lilian on 03/10/2021.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <initializer_list>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"


class Program {
    private:
        unsigned int program_id;

    public:
        Program() : program_id(glCreateProgram()) {};
        Program(const std::string & vertexShaderSourceFile, const std::string & fragmentShaderSourceFile);
        Program(const Program &) = delete;
        ~Program();

        void addShader(const Shader& shader, bool link = true);
        void addShader(const std::string & shaderSourceFile, ShaderType type, bool link = true);

        void Link() const;
        void Bind() const;
        static void Unbind();

        [[nodiscard]] int getUniformLocation(const std::string & uniformName, bool warn = true) const;
        template<typename T> bool setUniform(const std::string & uniformName, std::initializer_list<T> values, bool warn = true);
        bool setUniformMatrix4(const std::string & uniformName, const glm::mat4 & matrix, bool warn = true);
        bool setUniformMatrix4(const std::string & uniformName, const glm::mat4 *ptr, unsigned int n, bool warn = true);
};


#endif //PROGRAM_H
