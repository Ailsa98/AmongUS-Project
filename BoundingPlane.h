#ifndef _BOUNDING_PLANE_H_
#define _BOUNDING_PLANE_H_

#include "BoundingSphere.h"

class BoundingPlane {
private:
    glm::vec3 normal;
    glm::vec3 point;
    GLfloat distance;
    glm::vec3 newNorm;
    glm::vec3 newPt;

public:
    BoundingPlane(glm::vec3 norm, glm::vec3 pt) : normal(norm), point(pt) {
        newPt = point;
        newNorm = normal;
        distance = glm::dot(newPt, newNorm);
    };
    ~BoundingPlane(){};
    bool isCollide(BoundingSphere* bs) {
        float dist = glm::dot(bs->getPos(), newNorm) - distance;
        //std::cout << "dist: " << dist << std::endl;
        //std::cout << "r: " << bs->getRadius() << std::endl;
        if (dist <= bs->getRadius())
            return true;
        return false;
    };
    void update(glm::mat4 C) {
        normal = glm::vec3(C * glm::vec4(normal, 0));
        point = glm::vec3(C * glm::vec4(point, 1.0));
    };
    void draw(glm::mat4 C) {
        newNorm = glm::vec3(C * glm::vec4(normal, 0));
        newPt = glm::vec3(C * glm::vec4(point, 1));
        distance = glm::dot(newPt, newNorm);
    };
    void printPos() {
        std::cout << "newPt: "<< newPt.x << " " << newPt.y << " " << newPt.z << std::endl;
        std::cout << "newNorm: "<< newNorm.x << " " << newNorm.y << " " << newNorm.z << std::endl;
        std::cout << "distance: " << distance << std::endl;
    };
};

#endif
