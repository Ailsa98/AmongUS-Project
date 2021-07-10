#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "Node.h"
#include "Material.h"

class ParticleSystem : public Node {
private:
    struct Particle {
        //glm::vec3 position;
        glm::vec3 direction;
        GLfloat velocity;
        GLfloat life;
        
        Particle(glm::vec3 dir, GLfloat vel, GLfloat l) : direction(dir), velocity(vel), life(l) {}
    };
    
    static const int partNum = 100;
    static const int partLife = 1;
    GLfloat partSysLife = 3;
    GLfloat dt = 0.02;
    
    glm::vec3 color;
    glm::mat4 model;
    glm::mat4 newModel;
    
    GLuint VAO, VBO;
    
    std::vector<Particle> particles;
    std::vector<glm::vec3> positions;
    
    bool spawn = true;
    unsigned int lastUsed;
    unsigned int renewNum;

public:
    ParticleSystem();
    ~ParticleSystem();
    
    void update(glm::mat4 C);
    void draw(glm::mat4 C);
    void setColor(Material*& col) { color = col->color; };
    unsigned int firstUnused();
    void reSpawn(bool sp);
    void setSpawn(bool sp) { spawn = sp; };
};

#endif
