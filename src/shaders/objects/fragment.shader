#version 450 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoord;
out vec4 color;

//uniform sampler2D u_Texture;
uniform vec4 u_Color;

// Lighting
uniform vec3 u_pacmanPosition;
uniform vec3 u_lightPosition;

void main() {
    // TODO add lighting
    color = u_Color;
}