#ifndef _SPHERE_H_
#define _SPHERE_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Geometry.h"

class Sphere : public Geometry {
private:
    GLuint VAO, VBO1, VBO2, EBO;
    glm::vec3 _minVal, _maxVal, _center;
    
    glm::mat4 model;
    glm::mat4 newModel;
    
    unsigned int cubemapTexture;
    GLuint refShader;
    
    int stackCount = 30;
    int sectorCount = 30;
    int numsToDraw;
    
public:
    Sphere();
    ~Sphere();
    void draw(const GLuint shader, glm::mat4 C);
    void setTexture(unsigned int ID){ cubemapTexture = ID; };
    void setShader(GLuint shader){ refShader = shader; };
    void spin(float deg);
    void setColor(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shin){ return; };
    void update(glm::mat4 C){ model = C * model; }
};


#endif
