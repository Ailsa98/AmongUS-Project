#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <stdio.h>


class Transform : public Node {
private:
    glm::mat4 T;
    std::vector<Node *> children;

public:
	Transform(glm::mat4 M);
	~Transform();

    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
    void addChild(Node * child);
};

#endif
