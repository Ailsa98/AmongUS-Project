#include "Camera.h"

Camera::Camera(glm::vec3 eye, glm::vec3 look, glm::vec3 up) : eyePos(eye), lookAtPoint(look), upVector(up){
    view = glm::lookAt(eyePos, lookAtPoint, upVector);
    newEyePos = eyePos;
    newLookPos = lookAtPoint;
    newView = view;
}

Camera::~Camera() {
    
}

void Camera::update(glm::mat4 C) {
    eyePos = glm::vec3(C * glm::vec4(eyePos, 1));
    lookAtPoint = glm::vec3(C * glm::vec4(lookAtPoint, 1));
    newView = glm::lookAt(eyePos, lookAtPoint, upVector);
}

void Camera::draw(GLuint shaderProgram, glm::mat4 C) {
    newEyePos = glm::vec3(C * glm::vec4(eyePos, 1));
    newLookPos = glm::vec3(C * glm::vec4(lookAtPoint, 1));
    newView = glm::lookAt(newEyePos, newLookPos, glm::vec3(0, 1, 0));
}
