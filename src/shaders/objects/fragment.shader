#version 450 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texCoord;
out vec4 color;

//uniform sampler2D u_Texture;

// Lighting
uniform vec4 u_objectColor;
uniform vec3 u_pacmanPosition;

uniform vec3 u_lightPosition;
uniform vec3 u_lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform float u_ambiantIntensity = 1.0f;
uniform float u_diffuseIntensity = 0.0f;
uniform float u_specularIntensity = 0.0f;
uniform float alpha = 32;


void main() {
    vec3 ambiant, diffuse, specular;
    vec3 lightDirection = normalize(u_lightPosition - v_position);
    vec3 pacmanDirection = normalize(u_pacmanPosition - v_position);

    // Ambiant lighting
    ambiant = vec3(u_ambiantIntensity) * u_lightColor;

    // Diffuse lighting
    diffuse = u_diffuseIntensity * max(dot(v_normal, lightDirection), 0.0f) * u_lightColor;

    // Specular lighting
    vec3 reflected_ray = reflect(-lightDirection, v_normal);
    specular = u_specularIntensity * pow(max(dot(pacmanDirection, reflected_ray), 0.0f), alpha) * u_lightColor;

    vec3 phong = ambiant + diffuse + specular;
    color = vec4(phong * u_objectColor.rgb, u_objectColor.a);
}