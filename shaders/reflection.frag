#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 fragNormal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform samplerCube skybox;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main() {
    float ratio = 1.00 / 1.52;
    // viewPos - fragPos -> continuous texture
    vec3 I = normalize(fragPos - viewPos);
    // - -> upside down
    vec3 R = -refract(I, normalize(fragNormal), ratio);
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}
