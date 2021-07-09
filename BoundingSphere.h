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
    BoundingSphere(float r, glm::vec3 pos = glm::vec3(0, 0, 0)) : position(pos), radius(r){
        newPos = position;
    };
    ~BoundingSphere(){};
    void update(glm::mat4 C) {
        position = glm::vec3(C * glm::vec4(position, 1.0));
    };
    void draw(GLuint shaderProgram, glm::mat4 C) {
        newPos = glm::vec3(C * glm::vec4(position, 1.0));
    };
    glm::vec3 getPos(){
        return newPos;
    };
    float getRadius() {
        return radius;
    };
    GLfloat collide(BoundingSphere* bs) {
        glm::vec3 pos = bs->getPos();
        float diffx = pos.x - newPos.x;
        float diffy = pos.y - newPos.y;
        float diffz = pos.z - newPos.z;
        float minDist = radius + bs->getRadius();
        
        if (print) {
            std::cout << "diffx: " << diffx << " diffy: " << diffy << " diffz: " << diffz << std::endl;
            std::cout << "diff: " << diffz * diffz + diffy * diffy << " minDist: " << minDist * minDist << std::endl;
        }

        //return diffz * diffz + diffy * diffy <= minDist * minDist;

        float diff = diffz * diffz + diffy * diffy - minDist * minDist;
        //if (diff <= 0)  std::cout << "bs diff: " << diff << std::endl;
        if (diff >= 0) return sqrt(diff);
        else return -sqrt(abs(diff));
    };
    GLfloat isCollide(BoundingSphere* bs) {
        glm::vec3 pos = bs->getPos();
        //float diffx = pos.x - newPos.x;
        float diffy = pos.y - newPos.y;
        float diffz = pos.z - newPos.z;
        float minDist = radius + bs->getRadius();
        
        //std::cout << "diffx: " << diffx << " diffy: " << diffy << " diffz: " << diffz << std::endl;
        //std::cout << "diff: " << diffz * diffz + diffy * diffy << " minDist: " << minDist * minDist << std::endl;

        return diffz * diffz + diffy * diffy <= minDist * minDist;
    };
    void printPos() {
        std::cout << "newPos: " << newPos.x << " " << newPos.y << " " << newPos.z << std::endl;
    };
    
};

#endif
