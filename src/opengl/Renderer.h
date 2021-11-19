//
// Created by lilian on 03/10/2021.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Program.h"


class Renderer {
    public:
        static inline GLFWwindow* context;

        static bool init(unsigned int width, unsigned int height, const std::string& title, bool fullscreen = false);
        static void Clear();
        static void setClearColor(float r, float g, float b, float a = 1);
        static void Draw(const VertexArray & vao, const IndexBuffer & ibo, const Program & shader);
        static void DrawInstances(const VertexArray & vao, const IndexBuffer & ibo, const Program & shader, int n);
};


void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);


#endif //RENDERER_H
