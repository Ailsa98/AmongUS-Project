#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Node.h"

class Camera : public Node {
private:
    glm::mat4 view;
    glm::vec3 eyePos, lookAtPoint;
    GLuint refShader;
    
public:
    glm::mat4 newView;
    glm::vec3 newEyePos, newLookPos, upVector;
    
    Camera(glm::vec3 eye, glm::vec3 look, glm::vec3 up);
    ~Camera();

    void draw(glm::mat4 C);
    void update(glm::mat4 C);
    glm::vec3 getEyePos(){ return eyePos; };
    glm::vec3 getLookPos(){ return lookAtPoint; };
    glm::vec3 getUpVec(){ return upVector; };
    glm::mat4 getView(){ return view; };
    void setEyePos(glm::vec3 eye){ eyePos = eye; };
    void setLookPos(glm::vec3 look){ lookAtPoint = look; };
    void setUpVec(glm::vec3 up){ upVector = up; };
    void setView(glm::mat4 v){ view = v; };
};

#endif
