#ifndef _BOUNDING_PLANE_H_
#define _BOUNDING_PLANE_H_

#include "BoundingSphere.h"

class BoundingPlane : public Node {
private:
    glm::vec3 normal;
    glm::vec3 point;
    GLfloat distance;
    glm::vec3 newNorm;
    glm::vec3 newPt;

public:
    bool print = false;
    BoundingPlane(glm::vec3 norm, glm::vec3 pt) : normal(norm), point(pt) {
        newPt = point;
        newNorm = normal;
        distance = glm::dot(newPt, newNorm);
    };
    ~BoundingPlane(){};
    
    GLfloat collide(BoundingSphere* bs) {
        float dist = glm::dot(bs->getPos(), newNorm) - distance;
        if (print) {
            std::cout << "dist: " << dist << std::endl;
            std::cout << "r: " << bs->getRadius() << std::endl;
        }

        return dist - bs->getRadius();
    };
    bool isCollide(BoundingSphere* bs) {
        float dist = glm::dot(bs->getPos(), newNorm) - distance;
        
        /*
        if (dist <= bs->getRadius()){
            std::cout << "dist: " << dist << std::endl;
            std::cout << "r: " << bs->getRadius() << std::endl;
            //printPos();
        }
         */
        
        return dist <= bs->getRadius();
    };
    void update(glm::mat4 C) {
        normal = glm::vec3(C * glm::vec4(normal, 0));
        point = glm::vec3(C * glm::vec4(point, 1.0));
        distance = glm::dot(point, normal);
    };
    void draw(GLuint shaderProgram, glm::mat4 C) {
        newNorm = glm::normalize(glm::vec3(C * glm::vec4(normal, 0)));
        newPt = glm::vec3(C * glm::vec4(point, 1));
        distance = glm::dot(newPt, newNorm);
    };
    void printPos() {
        std::cout << "newPt: "<< newPt.x << " " << newPt.y << " " << newPt.z << std::endl;
        std::cout << "newNorm: "<< newNorm.x << " " << newNorm.y << " " << newNorm.z << std::endl;
        std::cout << "distance: " << distance << std::endl;
    };
    glm::vec3 getNorm() { return newNorm; };
    GLfloat getDist() { return distance; }
};

#endif
