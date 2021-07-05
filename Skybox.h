#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Skybox {
private:
    GLuint VAO;
    GLuint VBO, EBO;
    
    GLuint shaderProgram;
    glm::mat4 model;
        
    unsigned int cubemapTexture;
    
    std::vector<glm::ivec3> texCoords;
    
    std::string directory = "/Users/ailsafan/Desktop/demo/CSE167StarterCode-master/demo2/skybox/";
    std::vector<std::string> box {
        directory + "rainbow_right.png",
        directory + "rainbow_left.png",
        directory + "rainbow_up.png",
        directory + "rainbow_dn.png",
        directory + "rainbow_front.png",
        directory + "rainbow_back.png"
    };

public:
    Skybox(float size);
    ~Skybox();

    void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
    unsigned int loadCubemap(std::vector<std::string> faces);
    unsigned int getTexture(){ return cubemapTexture; };
    void rotate(float rotAngle, glm::vec3 rotAxis);
    void update();
};

#endif

