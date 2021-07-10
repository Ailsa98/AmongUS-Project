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
    BoundingPlane(glm::vec3 norm, glm::vec3 pt);
    ~BoundingPlane();
    
    GLfloat collide(BoundingSphere* bs);
    bool isCollide(BoundingSphere* bs);
    void update(glm::mat4 C);
    void draw(glm::mat4 C);
    void printPos() {
        std::cout << "newPt: "<< newPt.x << " " << newPt.y << " " << newPt.z << std::endl;
        std::cout << "newNorm: "<< newNorm.x << " " << newNorm.y << " " << newNorm.z << std::endl;
        std::cout << "distance: " << distance << std::endl;
    };
    glm::vec3 getNorm() { return newNorm; };
    GLfloat getDist() { return distance; }
};

#endif
