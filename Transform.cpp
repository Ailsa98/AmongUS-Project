#include "Transform.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Window.h"

Transform::Transform(glm::mat4 M) {
    T = M;
    newT = T;
}

Transform::~Transform() {
    for (Node* c : children) {
        delete c;
    }
    //delete this;
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C) {
    newT = C * T;
    for (int i = 0; i < children.size(); i++){
        children[i]->draw(shaderProgram, newT);
    }
}

void Transform::update(glm::mat4 C) {
    T = C * T;
}

void Transform::addChild(Node* child) {
    children.push_back(child);
}
