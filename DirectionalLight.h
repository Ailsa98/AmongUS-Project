#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "Node.h"

class DirectionalLight : public Node {
private:
    glm::vec3 lightColor;
    glm::vec3 lightDirection;
    
    /*
    glm::vec3 attenuation;
    // float constant;
    // float linear;
    // float quadratic;
     */
    
public:
    DirectionalLight(glm::vec3 color, glm::vec3 dir);
    ~DirectionalLight();

    void draw(glm::mat4 C);
    void update(glm::mat4 C);
    void rotate(float rotAngle, glm::vec3 rotAxis);
    glm::vec3 getDir(){ return lightDirection; };
    void setDir(glm::vec3 dir){ lightDirection = dir; };
};

#endif
