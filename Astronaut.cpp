#include "Astronaut.h"

Astronaut::Astronaut(std::string directory) {
    astroStill = new Geometry(directory + "amongus_astro_still.obj");
    astroRight = new Geometry(directory + "amongus_astro_moving1.obj");
    astroLeft = new Geometry(directory + "amongus_astro_moving2.obj");
    astro = {astroStill, astroRight, astroLeft};
    currAstro = 0;
    
    astroStill->update(glm::translate(glm::mat4(1), glm::vec3(0, 0.5, 0)));
    
    // r = 10^-39 -> dist > r after a move
    //GLfloat r = astroStill->getMaxDist();
    //bs = new BoundingSphere(0.8);
}

Astronaut::~Astronaut() {
    delete astroStill;
    delete astroRight;
    delete astroLeft;
    //delete bs;
}

void Astronaut::draw(glm::mat4 C) {
    astro[currAstro]->setShader(shader);
    astro[currAstro]->draw(C);
    //bs->draw(C);
}

void Astronaut::setColor(Material*& col) {
    astroStill->setColor(col);
    astroRight->setColor(col);
    astroLeft->setColor(col);
}

void Astronaut::useToon(bool use) {
    astroStill->useToon(use);
    astroRight->useToon(use);
    astroLeft->useToon(use);
};

void Astronaut::update(glm::mat4 C) {
    astroStill->update(C);
    astroRight->update(C);
    astroLeft->update(C);
    //bs->update(C);
};
