//
// Created by lilian on 03/10/2021.
//

#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>


std::string getFileContent(const std::string & filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string shaderTypeToString(ShaderType type) {
    switch (type) {
        case ShaderType::COMPUTE_SHADER: return "COMPUTE_SHADER";
        case ShaderType::VERTEX_SHADER: return "VERTEX_SHADER";
        case ShaderType::TESS_CONTROL_SHADER: return "TESS_CONTROL_SHADER";
        case ShaderType::TESS_EVALUATION_SHADER: return "TESS_EVALUATION_SHADER";
        case ShaderType::GEOMETRY_SHADER: return "GEOMETRY_SHADER";
        case ShaderType::FRAGMENT_SHADER: return "FRAGMENT_SHADER";
        default: return "UNDEFINED";
    }
}


Shader::Shader(const std::string &sourceFile, ShaderType shaderType) : type(shaderType) {
    std::string shaderSrc = getFileContent(sourceFile);
    const char* shaderSource = shaderSrc.c_str();
    shaderId = glCreateShader(static_cast<GLenum>(shaderType));
    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    Compile();
}

Shader::~Shader() {
    glDeleteShader(shaderId);
}

bool Shader::Compile() {
    glCompileShader(shaderId);
    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        std::cerr << "Failed to compile " << shaderTypeToString(type) << " shader:" << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(shaderId);
        return false;
    }
    return true;
}