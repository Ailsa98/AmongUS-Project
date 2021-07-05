#ifndef _ASTRONAUT_H_
#define _ASTRONAUT_H_

#include "Geometry.h"
#include "BoundingSphere.h"

class Astronaut : public Geometry {
private:
    // astronauts
    Geometry * astroStill;
    Geometry * astroLeft;
    Geometry * astroRight;
    std::vector<Geometry*> astro;
    int currAstro;
    
    BoundingSphere * bs;
    
public:
    Astronaut() { return; };
    Astronaut(std::string objFilename);
    ~Astronaut();
    
    void draw(GLuint shader, glm::mat4 C);
    void update(glm::mat4 C);
    //unsigned int loadTexture(std::string filename);
    void setColor(Material*& color);
    void useToon(bool use);
    void useAstro(int ind) { currAstro = ind; };
    BoundingSphere* getBS() { return bs; };
};

#endif
