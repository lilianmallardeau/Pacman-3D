#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
out vec3 v_position;
out vec3 v_normal;
out vec2 v_texCoord;

uniform mat4 u_viewProjMatrix;
uniform mat4 u_transformMatrices[1000];

void main() {
    gl_Position = u_viewProjMatrix * u_transformMatrices[gl_InstanceID] * vec4(position, 1);

    v_position = position;
    v_normal = normal;
    v_texCoord = texCoord;
}