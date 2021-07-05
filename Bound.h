#ifndef _BOUND_H_
#define _BOUND_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Bound {
    
public:
    virtual ~Bound() {};
    virtual void draw(glm::mat4 C) = 0;
    virtual void update(glm::mat4 C) = 0;
    virtual bool isCollide(Bound* bs) = 0;
};

#endif
