#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class PointCloud : public Object {
private:
    // read obj
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faces;
    
    // resize to fit
    GLfloat minX = std::numeric_limits<GLfloat>::max();
    GLfloat maxX = std::numeric_limits<GLfloat>::min();
    GLfloat minY = std::numeric_limits<GLfloat>::max();
    GLfloat maxY = std::numeric_limits<GLfloat>::min();
    GLfloat minZ = std::numeric_limits<GLfloat>::max();
    GLfloat maxZ = std::numeric_limits<GLfloat>::min();
    glm::vec3 center;
    GLfloat maxDist = std::numeric_limits<GLfloat>::min();
    
    // coloring
    static GLint colorMode;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    

	GLuint VAO, VBO1, VBO2, EBO;

public:
	PointCloud(std::string objFilename);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void spin(float deg);
    void rotateObj(float rotAngle, glm::vec3 rotAxis);
    void scale(GLfloat multiple);
    void changeColorMode();
    void setColor(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
    void translate(glm::vec3 trans);
};

#endif
