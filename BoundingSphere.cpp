#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(float r, glm::vec3 pos) : position(pos), radius(r){
    newPos = position;
}

BoundingSphere::~BoundingSphere(){
    
}

void BoundingSphere::update(glm::mat4 C) {
    position = glm::vec3(C * glm::vec4(position, 1.0));
}

void BoundingSphere::draw(glm::mat4 C) {
    newPos = glm::vec3(C * glm::vec4(position, 1.0));
}

GLfloat BoundingSphere::collide(BoundingSphere* bs) {
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
}

GLfloat BoundingSphere::isCollide(BoundingSphere* bs) {
    glm::vec3 pos = bs->getPos();
    //float diffx = pos.x - newPos.x;
    float diffy = pos.y - newPos.y;
    float diffz = pos.z - newPos.z;
    float minDist = radius + bs->getRadius();
    
    //std::cout << "diffx: " << diffx << " diffy: " << diffy << " diffz: " << diffz << std::endl;
    //std::cout << "diff: " << diffz * diffz + diffy * diffy << " minDist: " << minDist * minDist << std::endl;
    
    return diffz * diffz + diffy * diffy <= minDist * minDist;
}
