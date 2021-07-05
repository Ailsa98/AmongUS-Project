#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

class BoundingSphere {
private:
    glm::vec3 position;
    glm::vec3 newPos;
    float radius;
    
public:
    // !!!parameters with default values have to come at the end of the list
    BoundingSphere(float r, glm::vec3 pos = glm::vec3(0, 0, 0)) : position(pos), radius(r){
        newPos = position;
    };
    ~BoundingSphere(){};
    void update(glm::mat4 C) {
        position = glm::vec3(C * glm::vec4(position, 1.0));
    };
    void draw(glm::mat4 C) {
        newPos = glm::vec3(C * glm::vec4(position, 1.0));
    };
    glm::vec3 getPos(){
        return newPos;
    };
    float getRadius() {
        return radius;
    };
    bool isCollide(BoundingSphere* bs) {
        glm::vec3 pos = bs->getPos();
        float diffx = pos.x - newPos.x;
        float diffy = pos.y - newPos.y;
        float diffz = pos.z - newPos.z;
        float minDist = radius + bs->getRadius();
        
        //std::cout << "diffx: " << diffx << " diffy: " << diffy << " diffz: " << diffz << std::endl;
        //std::cout << "diff: " << diffz * diffz + diffy * diffy << " minDist: " << minDist * minDist << std::endl;
        
        if (diffz * diffz + diffy * diffy <= minDist * minDist)
            return true;
        return false;
    };
    void printPos() {
        std::cout << "newPos: " << newPos.x << " " << newPos.y << " " << newPos.z << std::endl;
    };
    
};

#endif
