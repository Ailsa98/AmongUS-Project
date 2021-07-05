#ifndef _LOBBY_H_
#define _LOBBY_H_

#include "Geometry.h"
//#include "BoundingSphere.h"
#include "BoundingPlane.h"

class Lobby : public Geometry {
private:
    // Lobby
    Geometry * lobby;
    
    // Bounding
    std::vector<BoundingSphere*> bSpheres;
    std::vector<BoundingPlane*> bPlanes;
    BoundingSphere* bsLeft, *bsRight;
    BoundingPlane* top, *bottom, *left, *right, *ltCorner, *rtCorner;
    
public:
    Lobby() { return; };
    Lobby(std::string objFilename);
    ~Lobby();
    
    void draw(GLuint shader, glm::mat4 C);
    void update(glm::mat4 C);
    //unsigned int loadTexture(std::string filename);
    void setColor(Material*& color);
    void useToon(bool use);
    void setTexture(GLuint texId);
    void useTex(bool use);
    void useRef(bool use);
    std::vector<BoundingSphere*> getBSpheres() { return bSpheres; };
    std::vector<BoundingPlane*> getBPlanes() { return bPlanes; };
    BoundingPlane * getTop() { return top; };
    BoundingPlane * getBot() { return bottom; };
    BoundingPlane * getLeft() { return left; };
    BoundingPlane * getRight() { return right; };
    BoundingPlane * getLtCor() { return ltCorner; };
    BoundingPlane * getRtCor() { return rtCorner; };
    BoundingSphere * getBSLt() { return bsLeft; };
    BoundingSphere * getBSRt() { return bsRight; };
    std::vector<BoundingPlane*> * getBP() { return &bPlanes; };
    std::vector<BoundingSphere*> * getBS() { return &bSpheres; };
};

#endif
