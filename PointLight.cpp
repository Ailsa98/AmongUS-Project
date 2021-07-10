#include "PointLight.h"

PointLight::PointLight(glm::vec3 color, glm::vec3 pos, float atten) : lightColor(color), lightPosition(pos), attenuation(atten) {
    
}

PointLight::~PointLight() {
    
}

void PointLight::draw(glm::mat4 C) {
    glm::vec3 newPos = glm::vec3(C * glm::vec4(lightPosition, 1.0));
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "light.lightCol"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shader, "light.lightPos"), 1, glm::value_ptr(newPos));
    glUniform1f(glGetUniformLocation(shader, "light.attenuation"), attenuation);
    glUniform1i(glGetUniformLocation(shader, "lightSource"), 0);
    glUseProgram(0);
}

void PointLight::update(glm::mat4 C) {
    lightPosition = glm::vec3(C * glm::vec4(lightPosition, 1.0));
}

void PointLight::rotate(float rotAngle, glm::vec3 rotAxis) {
    lightPosition = glm::vec3(glm::rotate(glm::mat4(1.0), rotAngle, rotAxis) * glm::vec4(lightPosition, 1.0));
}
