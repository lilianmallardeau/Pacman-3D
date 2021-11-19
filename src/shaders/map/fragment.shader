#version 450 core

in vec3 v_position;
in vec2 v_texCoord;
out vec4 color;

uniform sampler2D u_Texture;
uniform bool u_enableTexture = true;
uniform vec3 u_groundColor = vec3(0.2f, 0.2f, 0.2f);

// Lighting
uniform vec3 u_pacmanPosition;
uniform vec3 u_lightPosition;

void main() {
    // TODO add lighting
    if (u_enableTexture)
        color = texture(u_Texture, v_texCoord);
    else
        color = vec4(u_groundColor, 1.0f);
}