//
// Created by lilian on 03/10/2021.
//

#include "Program.h"

#include <iostream>
#include <string>
#include <initializer_list>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


Program::Program(const std::string & vertexShaderSourceFile, const std::string & fragmentShaderSourceFile) : Program() {
    addShader(vertexShaderSourceFile, ShaderType::VERTEX_SHADER, false);
    addShader(fragmentShaderSourceFile, ShaderType::FRAGMENT_SHADER, false);
    Link();
}

Program::~Program() {
    glDeleteProgram(program_id);
}

//NOLINTNEXTLINE(readability-make-member-function-const)
void Program::addShader(const Shader& shader, bool link) {
    glAttachShader(program_id, shader);
    if (link) Link();
}

void Program::addShader(const std::string &shaderSourceFile, ShaderType type, bool link) {
    addShader(Shader(shaderSourceFile, type), link);
    // Pb: shader will be deleted before program linking
}

void Program::Link() const {
    glLinkProgram(program_id);
}

void Program::Bind() const {
    glUseProgram(program_id);
}

void Program::Unbind() {
    glUseProgram(0);
}

int Program::getUniformLocation(const std::string & uniformName, bool warn) const {
    // TODO Uniform location cache
    int location = glGetUniformLocation(program_id, uniformName.c_str());
    if (location == -1 && warn)
        std::cout << "Warning! Uniform '" << uniformName << "' doesn't exist!" << std::endl;
    return location;
}

template<>
bool Program::setUniform<float>(const std::string &uniformName, std::initializer_list<float> values, bool warn) {
    Bind();
    switch (values.size()) {
        case 1: glUniform1f(getUniformLocation(uniformName, warn), data(values)[0]); return true;
        case 2: glUniform2f(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1]); return true;
        case 3: glUniform3f(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1], data(values)[2]); return true;
        case 4: glUniform4f(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1], data(values)[2], data(values)[3]); return true;
        default: return false;
    }
}

template<>
bool Program::setUniform<int>(const std::string &uniformName, std::initializer_list<int> values, bool warn) {
    Bind();
    switch (values.size()) {
        case 1: glUniform1i(getUniformLocation(uniformName, warn), data(values)[0]); return true;
        case 2: glUniform2i(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1]); return true;
        case 3: glUniform3i(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1], data(values)[2]); return true;
        case 4: glUniform4i(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1], data(values)[2], data(values)[3]); return true;
        default: return false;
    }
}

template<>
bool Program::setUniform<unsigned int>(const std::string &uniformName, std::initializer_list<unsigned int> values, bool warn) {
    Bind();
    switch (values.size()) {
        case 1: glUniform1ui(getUniformLocation(uniformName, warn), data(values)[0]); return true;
        case 2: glUniform2ui(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1]); return true;
        case 3: glUniform3ui(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1], data(values)[2]); return true;
        case 4: glUniform4ui(getUniformLocation(uniformName, warn), data(values)[0], data(values)[1], data(values)[2], data(values)[3]); return true;
        default: return false;
    }
}

//NOLINTNEXTLINE(readability-make-member-function-const)
bool Program::setUniformMatrix4(const std::string &uniformName, const glm::mat4 &matrix, bool warn) {
    Bind();
    glUniformMatrix4fv(getUniformLocation(uniformName, warn), 1, GL_FALSE, glm::value_ptr(matrix));
    return true;
}

//NOLINTNEXTLINE(readability-make-member-function-const)
bool Program::setUniformMatrix4(const std::string &uniformName, const glm::mat4 *ptr, unsigned int n, bool warn) {
    Bind();
    glUniformMatrix4fv(getUniformLocation(uniformName, warn), (int) n, GL_FALSE, reinterpret_cast<const GLfloat *>(ptr));
    return true;
}
