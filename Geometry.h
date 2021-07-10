#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <string>
#include <fstream>
#include <sstream>

#include "Node.h"
#include "Material.h"

class Geometry : public Node {
private:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> faces;
    
    bool hasTexture = false;
    
    // color
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    
    glm::vec3 color;
    
    // models
    glm::mat4 model;
    glm::mat4 newModel;
    
    // initialize position and size
    GLfloat minX = std::numeric_limits<GLfloat>::max();
    GLfloat maxX = std::numeric_limits<GLfloat>::min();
    GLfloat minY = std::numeric_limits<GLfloat>::max();
    GLfloat maxY = std::numeric_limits<GLfloat>::min();
    GLfloat minZ = std::numeric_limits<GLfloat>::max();
    GLfloat maxZ = std::numeric_limits<GLfloat>::min();
    glm::vec3 center;
    
    GLfloat maxDist = std::numeric_limits<GLfloat>::min();
    
    GLuint VAO, VBO, NBO, TBO, EBO;
    
    // Render
    unsigned int textureID;
    bool useTexture = false;
    bool useReflection = false;
    bool useToonShading = false;
    
public:
    Geometry() { return; };
    Geometry(std::string objFilename);
    ~Geometry();
    
    void draw(glm::mat4 C);
    void update(glm::mat4 C) { model = C * model; };
    //unsigned int loadTexture(std::string filename);
    void setColor(Material*& color);
    void setTexture(GLuint texId) { textureID = texId; };
    void useTex(bool use) { useTexture = use; };
    void useRef(bool use) { useReflection = use; };
    void useToon(bool use) { useToonShading = use; };
};

#endif
