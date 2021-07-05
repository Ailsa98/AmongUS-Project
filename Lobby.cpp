#include "Lobby.h"

Lobby::Lobby(std::string directory) {
    lobby = new Geometry(directory + "amongus_lobby.obj");
    top = new BoundingPlane(glm::vec3(0, 0, 1), glm::vec3(0, 0, 3));
    bottom = new BoundingPlane(glm::vec3(0, 0, -1), glm::vec3(0, 0, 20.5));
    left = new BoundingPlane(glm::vec3(1, 0, 0), glm::vec3(-16, 0, 0));
    right = new BoundingPlane(glm::vec3(-1, 0, 0), glm::vec3(16, 0, 0));
    ltCorner = new BoundingPlane(glm::normalize(glm::vec3(1, 0, -1)), glm::vec3(-15, 0, 17));
    rtCorner = new BoundingPlane(glm::normalize(glm::vec3(-1, 0, -1)), glm::vec3(15, 0, 17));
    bsLeft = new BoundingSphere(2.5, glm::vec3(-10, 0, 11.5));
    bsRight = new BoundingSphere(2.5, glm::vec3(11, 0, 8));
    bPlanes = {top, bottom, left, right, ltCorner, rtCorner};
    bSpheres = {bsLeft, bsRight};
}

Lobby::~Lobby() {
    delete lobby;
    delete top;
    delete bottom;
    delete left;
    delete right;
    delete ltCorner;
    delete rtCorner;
    delete bsLeft;
    delete bsRight;
}

void Lobby::draw(GLuint shader, glm::mat4 C) {
    lobby->draw(shader, C);
    for (auto bp : bPlanes) {
        bp->draw(C);
    }
    for (auto bs : bSpheres) {
        bs->draw(C);
    }
}

void Lobby::setColor(Material*& col) {
    lobby->setColor(col);
}

void Lobby::useToon(bool use) {
    lobby->useToon(use);
};

void Lobby::setTexture(GLuint texId) {
    lobby->setTexture(texId);
};
void Lobby::useTex(bool use) {
    lobby->useTex(use);
};
void Lobby::useRef(bool use) {
    lobby->useRef(use);
};

void Lobby::update(glm::mat4 C) {
    lobby->update(C);
};

