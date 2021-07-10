#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

#include "Node.h"

class BoundingSphere : public Node {
private:
    glm::vec3 position;
    glm::vec3 newPos;
    float radius;
    
public:
    bool print = false;
    // !!!parameters with default values have to come at the end of the list
    BoundingSphere(float r, glm::vec3 pos = glm::vec3(0, 0, 0));
    ~BoundingSphere();
    void update(glm::mat4 C);
    void draw(glm::mat4 C);
    GLfloat collide(BoundingSphere* bs);
    GLfloat isCollide(BoundingSphere* bs);
    glm::vec3 getPos(){ return newPos; };
    float getRadius(){ return radius; };
    void printPos() {
        std::cout << "newPos: " << newPos.x << " " << newPos.y << " " << newPos.z << std::endl;
    };
    
};

#endif
