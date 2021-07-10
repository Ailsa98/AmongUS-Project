#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 dir) : lightColor(color), lightDirection(dir) {
    
}

DirectionalLight::~DirectionalLight() {
    
}

void DirectionalLight::draw(glm::mat4 C) {
    glm::vec3 newDir = glm::vec3(C * glm::vec4(lightDirection, 1.0));
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "light.lightCol"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shader, "light.direction"), 1, glm::value_ptr(newDir));
    glUniform1i(glGetUniformLocation(shader, "lightSource"), 1);
    glUseProgram(0);
}

// 0.0?
void DirectionalLight::update(glm::mat4 C) {
    lightDirection = glm::vec3(C * glm::vec4(lightDirection, 1.0));
}

void DirectionalLight::rotate(float rotAngle, glm::vec3 rotAxis) {
    lightDirection = glm::vec3(glm::rotate(glm::mat4(1.0), rotAngle, rotAxis) * glm::vec4(lightDirection, 1.0));
}
