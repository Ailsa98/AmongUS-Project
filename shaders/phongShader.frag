#version 330 core
// This is a sample fragment shader.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    
    vec3 color;
};

struct PointLight {
    vec3 lightPos;
    vec3 lightCol;
    float attenuation;
    // vec3 attenuation;
    vec3 direction;
};

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;

uniform PointLight light;
uniform Material material;
uniform vec3 viewPos;

uniform sampler2D textureID;
uniform int useTexture;
uniform int useToon;
uniform int lightSource;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main() {
    vec3 objColor;
    
    // Ambient
    vec3 ambient = light.lightCol * material.ambient;

    // Diffuse
    vec3 lightDir;
    if (lightSource == 0)
        lightDir = normalize(light.lightPos - fragPos);
    else if (lightSource == 1)
        lightDir = normalize(-light.direction);
        
    vec3 diffuse = vec3(0);
    float diff = max(dot(fragNormal, lightDir), 0.0);
    if (useTexture == 1)
        diffuse = vec3(texture(textureID, fragTexCoord));
    else if (useToon == 0)
        diffuse = (diff * material.diffuse) * light.lightCol;

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    
    vec3 specular = vec3(0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    if (useToon == 0) {
        specular = light.lightCol * (spec * material.specular);
    }

    // Attenuation
    float attenuation;
    
    if (useToon == 1) {
        // intensity
        float intensity = diff * spec;
        if (intensity > 0.95)
            objColor = 1.0 * material.color;
        else if (intensity > 0.5)
            objColor = 0.7 * material.color;
        else if (intensity > 0.05)
            objColor = 0.35 * material.color;
        else
            objColor = 0.1 * material.color;
        
        float edge = max(dot(viewDir, fragNormal), 0.0);
        
        if (edge < 0.5)
            objColor = vec3(0.0, 0.0, 0.0);
        
    }
    
    else if (lightSource != 1) {
        float dist = length(light.lightPos - fragPos);
        attenuation = 1.0f / light.attenuation / dist;
        objColor = attenuation * (ambient + diffuse + specular);
    }
    else {
        objColor = ambient + diffuse + specular;
    }

    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(objColor, 1);
}
