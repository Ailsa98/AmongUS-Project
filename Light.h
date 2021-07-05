#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Node.h"

class Light : public Node {
    
public:
    Light();
    ~Light();

    virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;
    virtual void update(glm::mat4 C) = 0;
};

#endif
