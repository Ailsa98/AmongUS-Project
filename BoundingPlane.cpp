#include "BoundingPlane.h"

BoundingPlane::BoundingPlane(glm::vec3 norm, glm::vec3 pt) : normal(norm), point(pt) {
    newPt = point;
    newNorm = normal;
    distance = glm::dot(newPt, newNorm);
}

BoundingPlane::~BoundingPlane(){
    
};

GLfloat BoundingPlane::collide(BoundingSphere* bs) {
    float dist = glm::dot(bs->getPos(), newNorm) - distance;
    if (print) {
        std::cout << "dist: " << dist << std::endl;
        std::cout << "r: " << bs->getRadius() << std::endl;
    }
    
    return dist - bs->getRadius();
}

bool BoundingPlane::isCollide(BoundingSphere* bs) {
    float dist = glm::dot(bs->getPos(), newNorm) - distance;
    
    /*
     if (dist <= bs->getRadius()){
     std::cout << "dist: " << dist << std::endl;
     std::cout << "r: " << bs->getRadius() << std::endl;
     //printPos();
     }
     */
    
    return dist <= bs->getRadius();
}

void BoundingPlane::update(glm::mat4 C) {
    normal = glm::vec3(C * glm::vec4(normal, 0));
    point = glm::vec3(C * glm::vec4(point, 1.0));
    distance = glm::dot(point, normal);
}

void BoundingPlane::draw(glm::mat4 C) {
    newNorm = glm::normalize(glm::vec3(C * glm::vec4(normal, 0)));
    newPt = glm::vec3(C * glm::vec4(point, 1));
    distance = glm::dot(newPt, newNorm);
}
