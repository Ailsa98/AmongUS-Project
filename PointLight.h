#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Node.h"

class PointLight : public Node {
private:
    glm::vec3 lightColor;
    glm::vec3 lightPosition;
    float attenuation;
    
    /*
    glm::vec3 attenuation;
    // float constant;
    // float linear;
    // float quadratic;
     */
    
public:
    PointLight(glm::vec3 color, glm::vec3 pos, float atten);
    ~PointLight();

    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
    void rotate(float rotAngle, glm::vec3 rotAxis);
    glm::vec3 getPos(){ return lightPosition; };
    void setPos(glm::vec3 pos){ lightPosition = pos; };
};

#endif
