#include "Transform.h"

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

void Transform::draw(glm::mat4 C) {
    newT = C * T;
    for (int i = 0; i < children.size(); i++){
        if (display[i]) children[i]->draw(newT);
    }
}

void Transform::update(glm::mat4 C) {
    T = C * T;
}

void Transform::addChild(Node* child) {
    children.push_back(child);
    display.push_back(true);
}
