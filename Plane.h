#ifndef _PLANE_H_
#define _PLANE_H_

#include "Node.h"

#include <vector>

class Plane : public Node
{
private:
	GLuint VAO;
	GLuint VBO, EBO;
    
    glm::mat4 model;
    

public:
	Plane();
	~Plane();

	void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C){};
};

#endif
