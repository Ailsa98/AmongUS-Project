#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>


class Transform : public Node {
private:
    glm::mat4 T;
    glm::mat4 newT;
    std::vector<Node *> children;
    std::vector<bool> display;

public:
	Transform(glm::mat4 M);
	~Transform();

    void draw(glm::mat4 C);
    void update(glm::mat4 C);
    void addChild(Node * child);
    void displayToggle(int i, bool show) { display[i] = show; };
};

#endif
