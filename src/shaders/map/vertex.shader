#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
out vec3 v_position;
out vec2 v_texCoord;

uniform mat4 u_viewProjMatrix;

void main() {
    gl_Position = u_viewProjMatrix * vec4(position, 1);

    v_position = position;
    v_texCoord = texCoord;
}