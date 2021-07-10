#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define pi 3.1415926

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to Render
Geometry * Window::lobby;
std::vector<Astronaut*> Window::astros = std::vector<Astronaut*>(12);

// Transforms
Transform * Window::world;
Transform * Window::astroSys;
Transform * Window::camCtrl;
Transform * Window::lob;
std::vector<Transform*> Window::astroRot = std::vector<Transform*>(12);
std::vector<Transform*> Window::astroMov = std::vector<Transform*>(12);

// Trackball
bool Window::leftPress = false;
glm::vec3 Window::prevPos;

// Render control
bool Window::useTex = true;
bool Window::useToon = true;
int Window::lightSrc = 0;

// Default filepath
std::string Window::directory = "/Users/ailsafan/Desktop/demo/demo3/obj/";

// Lights
//PointLight * Window::ptLight;
DirectionalLight * Window::dirLight;
Node * currLight;

// Colors
Material * Window::black = new Material(glm::vec3(62, 71, 78) / 255.f);
Material * Window::blue = new Material(glm::vec3(19, 46, 209) / 255.f);
Material * Window::brown = new Material(glm::vec3(113, 73, 29) / 255.f);
Material * Window::cyan = new Material(glm::vec3(57, 254, 221) / 255.f);
Material * Window::darkGreen = new Material(glm::vec3(19, 128, 44) / 255.f);
Material * Window::lime = new Material(glm::vec3(78, 239, 56) / 255.f);
Material * Window::orange = new Material(glm::vec3(241, 125, 12) / 255.f);
Material * Window::pink = new Material(glm::vec3(236, 84, 187) / 255.f);
Material * Window::purple = new Material(glm::vec3(108, 47, 188) / 255.f);
Material * Window::red = new Material(glm::vec3(197, 18, 17) / 255.f);
Material * Window::white = new Material(glm::vec3(214, 223, 241) / 255.f);
Material * Window::yellow = new Material(glm::vec3(246, 246, 87) / 255.f);
Material * Window::silver = new Material(glm::vec3(0.19225), glm::vec3(0.50754), glm::vec3(0.508273), 0.4);
std::vector<Material*> Window::astroColors = {
    black, blue, brown, cyan, darkGreen, lime, orange, pink, purple, red, white, yellow, silver
};

// Cameras
Camera * Window::camera;

// Projection matrix:
glm::mat4 Window::projection;

// Shader Program ID
GLuint Window::phongShader;
GLuint Window::partShader;
GLuint Window::refShader;

// Movement handle
std::vector<GLfloat> Window::faceDir(12, 0);
std::vector<glm::vec3> Window::velocity(12);

int Window::currAstro = 0;
GLfloat Window::userSpeed = 0.4;

std::vector<GLfloat> Window::spawnTime(12);
std::vector<GLfloat> Window::pauseTime(12);
std::vector<GLfloat> Window::despawnTime(12);
std::vector<bool> Window::isMove(12);
std::vector<bool> Window::isShown(12);
std::vector<bool> Window::useRight(12);
std::vector<GLuint> Window::cntFrame(12);
GLuint Window::astroCnt = 1;

GLfloat Window::startTime;

// Bounds
std::vector<BoundingSphere*> Window::astroSpheres(12);
std::vector<BoundingPlane*> Window::lobPlanes;
std::vector<BoundingSphere*> Window::lobSpheres;
BoundingSphere* Window::bsLeft, * Window::bsRight;
BoundingPlane* Window::top, * Window::bottom, * Window::left, * Window::right, * Window::ltCorner, * Window::rtCorner;

// Particles
std::vector<ParticleSystem*> Window::partSys(12);

bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    phongShader = LoadShaders("shaders/phongShader.vert", "shaders/phongShader.frag");
    partShader = LoadShaders("shaders/particle.vert", "shaders/particle.frag");
    refShader = LoadShaders("shaders/reflection.vert", "shaders/reflection.frag");
    
    // Check the shader program.
    if (!phongShader || !partShader || !refShader) {
        std::cerr << "Failed to initialize shader programs" << std::endl;
        return false;
    }
    
    return true;
}

bool Window::initializeObjects() {
    // Initialize point lights
    //ptLight = new PointLight(glm::vec3(1.f), glm::vec3(100, 50, 0), 0.01);
    //ptLight->setShader(phongShader);
    dirLight = new DirectionalLight(glm::vec3(1.f), glm::vec3(-100, 200, 0));
    dirLight->setShader(phongShader);
    currLight = dirLight;
    
    // Initialize cameras
    camera = new Camera(glm::vec3(50, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camCtrl = new Transform(glm::mat4(1));
    camCtrl->addChild(camera);
    
    // Initialize objects
    glm::mat4 model = glm::mat4(1);
    lobby = new Geometry(directory + "amongus_lobby.obj");
    lobby->setColor(silver);
    lobby->setColor(white);
    lobby->setTexture(loadTexture(directory + "amongus_lobby.png"));
    lobby->useTex(useToon);
    lobby->setShader(phongShader);
    //lobby->useToon(0);
    model = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0, 0, 1));
    model = glm::translate(model, glm::vec3(0, 0, -5));
    model = glm::scale(model, glm::vec3(1.1));
    lobby->update(model);
    
    //model = glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 0, 1));
    // 0.35 -> attach the lower floor
    //model = glm::translate(glm::mat4(1), glm::vec3(0.5, 0, 0));
    astroSys = new Transform(glm::mat4(1));
    
    // Initialize the system
    for (int i = 0; i < 12; ++i) {
        astroRot[i] = new Transform(glm::mat4(1));
        astroMov[i] = new Transform(glm::mat4(1));
        
        astroSpheres[i] = new BoundingSphere(0.8);
        
        astros[i] = new Astronaut(directory);
        astros[i]->setColor(astroColors[i]);
        astros[i]->setShader(phongShader);
        astros[i]->useToon(useToon);
        
        partSys[i] = new ParticleSystem();
        partSys[i]->setColor(astroColors[i]);
        partSys[i]->setShader(partShader);
        
        astroRot[i]->addChild(astros[i]);
        astroMov[i]->addChild(astroRot[i]);
        astroMov[i]->addChild(astroSpheres[i]);
        astroMov[i]->addChild(partSys[i]);
        astroSys->addChild(astroMov[i]);
        astroSys->displayToggle(i, false);
    }
    astroSys->displayToggle(currAstro, true);
    isShown[currAstro] = true;

    // bounds
    top = new BoundingPlane(glm::vec3(0, -1, 0), glm::vec3(0, 0, 0));
    bottom = new BoundingPlane(glm::vec3(0, 1, 0), glm::vec3(0, -18, 0));
    left = new BoundingPlane(glm::vec3(0, 0, -1), glm::vec3(0, 0, 17.5));
    right = new BoundingPlane(glm::vec3(0, 0, 1), glm::vec3(0, 0, -17.5));
    ltCorner = new BoundingPlane(glm::normalize(glm::vec3(0, 1, -1)), glm::vec3(0, -13, 17.5));
    rtCorner = new BoundingPlane(glm::normalize(glm::vec3(0, 1, 1)), glm::vec3(0, -13, -17.5));
    lobPlanes = { top, bottom, left, right, ltCorner, rtCorner };
    //rtCorner->print = true;
    
    bsLeft = new BoundingSphere(2.5, glm::vec3(1, -7.5, 10.5));
    bsRight = new BoundingSphere(2.5, glm::vec3(1, -4, -11.5));
    lobSpheres = { bsLeft, bsRight };
    //bsLeft->print = true;
    
    //model = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0));
    
    // Randomize the initial direction
    srand((unsigned)time(NULL));
    for (int i = 0; i < 12; ++i) {
        if (i == currAstro) {
            faceDir[i] = rand() % 4 * 90;
        } else {
            faceDir[i] = rand() % 360;
        }
        velocity[i] = glm::vec3(0, -sin(glm::radians(faceDir[i])), cos(glm::radians(faceDir[i])));
    }
    
    //faceDir[0] = 30;
    //std::cout << faceDir[currAstro] << std::endl;
    
    
    // Randomize the initial position
    // 14 - - -14
    //      |
    //     -16
    GLfloat x, y;
    srand((unsigned)time(NULL));
    for (int i = 0; i < 12; ++i) {
        astroRot[i]->update(glm::rotate(glm::mat4(1), glm::radians(faceDir[i]), glm::vec3(0, 1, 0)));
        astroRot[i]->update(glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 0, 1)));
        x = rand() % 1600;
        x = x / 100;
        y = rand() % 2800;
        y = y / 100 - 14;
        if (i != currAstro) astroMov[i]->update(glm::translate(glm::mat4(1), glm::vec3(0, -x, y)));
    }
    astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, -8, 0)));
    
    // Build relationships
    world = new Transform(glm::mat4(1));
    
    lob = new Transform(glm::mat4(1));
    lob->addChild(lobby);
    lob->addChild(top);
    lob->addChild(bsLeft);
    
    world->addChild(currLight);
    world->addChild(camera);
    world->addChild(astroSys);
    world->addChild(lob);
    
    // Random movement
    srand((unsigned)time(NULL));
    GLfloat stTime;
    
    startTime = glfwGetTime();
    
    for (int i = 0; i < 12; ++i) {
        if (i == currAstro) continue;
        stTime = rand() % 60 + 10;
        spawnTime[i] = (GLfloat)stTime / 10.0f;
    }
    
    //astros[0]->getBS()->printPos = true;
    
    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete world;
    
    // Delete the shader program.
    glDeleteProgram(phongShader);
    glDeleteProgram(partShader);
    glDeleteProgram(refShader);
}

GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }
    
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);
    
#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    
    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // Make the context of the window.
    glfwMakeContextCurrent(window);
    
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    
    // Initialize GLEW.
    if (glewInit()) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif
    
    // Set swap interval to 1.
    glfwSwapInterval(0);
    
    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);
    
    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);
    
    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(60.0),
                                          double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback() {
    for (int i = 0; i < 12; ++i) {
        // move the npc
        if (i != currAstro) {
            npcMove(i);
            if (isMove[i]) {
                handleCollision(i);
                astroMov[i]->update(glm::translate(glm::mat4(1), velocity[i] * 0.05f));
                partSys[i]->update(glm::mat4(1));
            }
        }
        // change models during the movement
        if (isMove[i]) {
            ++cntFrame[i];
            if (cntFrame[i] >= 20) {
                astros[i]->useAstro(useRight[i] + 1);
                useRight[i] = !useRight[i];
                cntFrame[i] = 0;
            }
        }
        else astros[i]->useAstro(0);
    }
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the objects
    glUseProgram(phongShader);
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "view"), 1, GL_FALSE, glm::value_ptr(camera->newView));
    glUniform3fv(glGetUniformLocation(phongShader, "viewPos"), 1, glm::value_ptr(camera->newEyePos));
    
    glUseProgram(partShader);
    glUniformMatrix4fv(glGetUniformLocation(partShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(partShader, "view"), 1, GL_FALSE, glm::value_ptr(camera->newView));
    
    world->draw(glm::mat4(1));
    //lobby->getBSLt()->printPos();
    
    glUseProgram(0);
    
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Check for a key press.
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        isMove[currAstro] = true;
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
                
            case GLFW_KEY_UP:
                astroRot[currAstro]->update(glm::rotate(glm::mat4(1), glm::radians(270.f - faceDir[currAstro]), glm::vec3(1, 0, 0)));
                faceDir[currAstro] = 270;
                velocity[currAstro] = glm::vec3(0, -sin(glm::radians(faceDir[currAstro])), cos(glm::radians(faceDir[currAstro])));
                
                //astros[0]->getBS()->printPos();
                
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, userSpeed, 0)));
                /*
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0,
                                                                                   -sin(glm::radians(faceDir[currAstro])),
                                                                                   cos(glm::radians(faceDir[currAstro]))) * userSpeed));
                */
                world->draw(glm::mat4(1));
                
                if (checkCollision(currAstro)) {
                    astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, -userSpeed, 0)));
                }
                
                //handleCollision(currAstro);
                
                /*
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0,
                                                                                   -sin(glm::radians(faceDir[currAstro])),
                                                                                   cos(glm::radians(faceDir[currAstro]))) * userSpeed));
                
                world->draw(phongShader, glm::mat4(1));
                 
                if (checkCollision(currAstro)) {
                    astroMov[currAstro]->update(glm::translate(glm::mat4(1),
                                                               glm::vec3(-sin(glm::radians(faceDir[currAstro])),
                                                                         0,
                                                                         -cos(glm::radians(faceDir[currAstro])))
                                                               * userSpeed));
                }
                 */
                
                break;
                
            case GLFW_KEY_DOWN:
                astroRot[currAstro]->update(glm::rotate(glm::mat4(1), glm::radians(90.f - faceDir[currAstro]), glm::vec3(1, 0, 0)));
                faceDir[currAstro] = 90;
                velocity[currAstro] = glm::vec3(0, -sin(glm::radians(faceDir[currAstro])), cos(glm::radians(faceDir[currAstro])));
                
                //astros[0]->getBS()->printPos();
                //handleCollision(currAstro);
                
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, -userSpeed, 0)));
                /*
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0,
                                                                                   -sin(glm::radians(faceDir[currAstro])),
                                                                                   cos(glm::radians(faceDir[currAstro]))) * userSpeed));
                */
                world->draw(glm::mat4(1));
                
                
                if (checkCollision(currAstro)) {
                    astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, userSpeed, 0)));
                }
                
                //handleCollision(currAstro);
                
                break;
                
            case GLFW_KEY_LEFT:
                astroRot[currAstro]->update(glm::rotate(glm::mat4(1), glm::radians(0.f - faceDir[currAstro]), glm::vec3(1, 0, 0)));
                faceDir[currAstro] = 0;
                velocity[currAstro] = glm::vec3(0, -sin(glm::radians(faceDir[currAstro])), cos(glm::radians(faceDir[currAstro])));
                
                //astros[0]->getBS()->printPos();
                //handleCollision(currAstro);
                
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, userSpeed)));
                /*
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0,
                                                                                   -sin(glm::radians(faceDir[currAstro])),
                                                                                   cos(glm::radians(faceDir[currAstro]))) * userSpeed));
                */
                world->draw(glm::mat4(1));
                
                if (checkCollision(currAstro)) {
                    astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, -userSpeed)));
                }
                
                //handleCollision(currAstro);
                
                break;
                
            case GLFW_KEY_RIGHT:
                astroRot[currAstro]->update(glm::rotate(glm::mat4(1), glm::radians(180.f - faceDir[currAstro]), glm::vec3(1, 0, 0)));
                faceDir[currAstro] = 180;
                velocity[currAstro] = glm::vec3(0, -sin(glm::radians(faceDir[currAstro])), cos(glm::radians(faceDir[currAstro])));
                
                //astros[0]->getBS()->printPos();
                //handleCollision(currAstro);
                
                astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, -userSpeed)));
                /*
                 astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0,
                                                                                    -sin(glm::radians(faceDir[currAstro])),
                                                                                    cos(glm::radians(faceDir[currAstro]))) * userSpeed));
                 */
                world->draw(glm::mat4(1));
                
                if (checkCollision(currAstro)) {
                    astroMov[currAstro]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, userSpeed)));
                }
                
                //handleCollision(currAstro);
                
                break;
                
            default:
                break;
        }
    }
    if (action == GLFW_RELEASE) {
        isMove[currAstro] = false;
    }
}

/**
 * Detect the left-button of the mouse being depressed.
 * Keep track of the last known mouse position (mapped to the hemi-sphere).
 */
void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        leftPress = true;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // Map the mouse position to a logical sphere location.
        prevPos = trackBallMapping(window, x, y);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        leftPress = false;
    }
}

/**
 * Treat the mouse position as the projection of a point on the hemi-sphere down to the image plane (along the z-axis),
 * and determine that point on the hemi-sphere.
 */
glm::vec3 Window::trackBallMapping(GLFWwindow* window, double xPos, double yPos) {
    // Mapping
    glm::vec3 v;
    // !!! currWidth != Window::width
    int currWidth, currHeight;
    glfwGetWindowSize(window, &currWidth, &currHeight);
    v.x = (2.0 * xPos - currWidth) / currWidth;
    v.y = (currHeight - 2.0 * yPos) / currHeight;
    v.z = 0.0;
    float d = glm::length(v);
    d = (d < 1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d * d);
    glm::normalize(v); // Still need to normalize, since we only capped d, not v.
    return v;
}

/**
 * Detect the mouse movements and update the display.
 */
void Window::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    if (leftPress) {
        // Detect the mouse movement
        glm::vec3 currPos = trackBallMapping(window, xPos, yPos); // Map the mouse position to a logical
        glm::vec3 direction = currPos - prevPos;
        float velocity = glm::length(direction);
        // If little movement - do nothing.
        if(velocity > 0.0001) {
            // Determine the great circle connecting the old and the current mouse-hemi-sphere point.
            // Calculate the normal to this plane. This will be the axis about which to rotate.
            glm::vec3 rotAxis = glm::cross(prevPos, currPos);
            rotAxis = glm::vec3(0, 0, rotAxis[0]);
            // Prevent empty screen
            if (rotAxis[0] == 0) {
                rotAxis[0] = 0.0000000000000001;
            }
            float rotAngle = velocity * 10;
            glm::vec3 eye = camera->getEyePos();
            camera->setEyePos(glm::vec3(glm::rotate(glm::radians(rotAngle), rotAxis) * glm::vec4(eye, 1.0)));
        }
        prevPos = currPos;
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    glm::vec3 eye = camera->getEyePos();
    GLfloat multiLt = 0.1 * yoffset;
    eye.x -= multiLt;
    camera->setEyePos(eye);
}

unsigned int Window::loadTexture(std::string texLocation) {
    unsigned int texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bi-linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for both filtering modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture edge mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // 3d texture
    
    // nrChannels = number of color channels
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    // data[0] = first px's R, data[1] = first px's G, data[2] = first px's B, data[3] = 2nd px's R, etc.
    unsigned char* data = stbi_load(texLocation.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // Loads image into OpenGL texture in GPU memory
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::string fail_reason = "";
        if (stbi_failure_reason())
            fail_reason = stbi_failure_reason();
        std::cout << "Texture failed to load at path: " << texLocation << " due to reason: " << fail_reason << std::endl;
    }
    stbi_image_free(data);
    
    std::cout << "Load texture done." << std::endl;
    
    return texId;
}

bool Window::checkCollision(int curr) {
    // walls
    for (auto bp : lobPlanes) {
        if (bp->isCollide(astroSpheres[curr])) {
            return true;
        }
    }
    
    // boxes
    for (auto bs : lobSpheres) {
        if (bs->isCollide(astroSpheres[curr])) {
            return true;
        }
    }
    
    // astronauts
    for (int i = 0; i < 12; ++i) {
        if (i == curr || !isShown[i])  continue;
        if (astroSpheres[i]->isCollide(astroSpheres[curr])) {
            return true;
        }
    }
    
    return false;
}

void Window::handleCollision(int curr, bool initialize){
    
    auto handleBP = [&](BoundingPlane* bp) -> glm::vec3 {
        glm::vec3 dir = glm::normalize(velocity[curr]);
        // use normal in the same coordinates as direction
        //dir = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 0, 1)) * glm::vec4(dir, 0);
        
        //glm::vec3 norm = glm::vec4(bp->getNorm(), 0) * glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 0, 1));
        glm::vec3 norm = bp->getNorm();
        glm::vec3 newDir = glm::reflect(dir, norm);
        //newDir = glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 0, 1)) * glm::vec4(newDir, 0);
        /*
         std::cout << "dir: " << dir.x << " " << dir.y << " " << dir.z << std::endl;
         std::cout << "normal: " << norm.x << " " << norm.y << " " << norm.z << std::endl;
         std::cout << "newDir: " << newDir.x << " " << newDir.y << " " << newDir.z << std::endl;
         */
        return newDir;
    };
    
    auto handleBS = [&](BoundingSphere* bs) -> glm::vec3 {
        glm::vec3 dir = glm::normalize(velocity[curr]);
        glm::vec3 diff = astroSpheres[curr]->getPos() - bs->getPos();
        diff.x = 0;
        /*
         std::cout << "bs: " << bs->getPos().x << " " << bs->getPos().y << " " << bs->getPos().z << std::endl;
         std::cout << "astro: " << astroSpheres[curr]->getPos().x << " " << astroSpheres[curr]->getPos().y << " " << astroSpheres[curr]->getPos().z << std::endl;
         */
        //diff = glm::normalize(diff);
        //glm::vec3 norm = glm::vec4(diff, 0) * glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0));
        //norm = glm::normalize(glm::vec3(0, norm.y, norm.z));
        glm::vec3 norm = glm::normalize(diff);
        glm::vec3 newDir = glm::reflect(dir, norm);
        //newDir.y = 0;
        /*
         std::cout << "dir: " << dir.x << " " << dir.y << " " << dir.z << std::endl;
         std::cout << "diff: " << diff.x << " " << diff.y << " " << diff.z << std::endl;
         std::cout << "normal: " << norm.x << " " << norm.y << " " << norm.z << std::endl;
         std::cout << "newDir: " << newDir.x << " " << newDir.y << " " << newDir.z << std::endl;
         */
        return newDir;
    };
    
    GLfloat diff;
    glm::vec3 newDir;
    glm::vec3 newTrans;
    GLfloat angle1, angle2, angle;
    
    // walls
    for (auto bp : lobPlanes) {
        diff = bp->collide(astroSpheres[curr]);
        if (diff <= 0) {
            newDir = handleBP(bp);
            newTrans = newDir * (0.01f - diff);
            //std::cout << "newTrans: " << newTrans.x << " " << newTrans.y << " " << newTrans.z << std::endl;
            astroMov[curr]->update(glm::translate(glm::mat4(1), newTrans));
            // asin(0) = 0 / 180, acos(0) = +-90
            angle1 = asin(-newDir.y);
            angle2 = acos(newDir.z);
            if (abs(angle1 - angle2) < 0.1 || abs(angle1 + angle2) < 0.1) angle = angle1;
            else angle = pi - angle1;
            
            //std::cout << "bp: faceDir: " << faceDir[curr] << " diff: " << diff << std::endl;
            astroRot[curr]->update(glm::rotate(glm::mat4(1), angle - glm::radians(faceDir[curr]), glm::vec3(1, 0, 0)));
            faceDir[curr] = glm::degrees(angle);
            velocity[curr] = newDir;
            
            //std::cout << "angle: " << angle << std::endl;
            world->draw(glm::mat4(1));
        }
    }
    
    // boxes
    for (auto bs : lobSpheres) {
        diff = bs->collide(astroSpheres[curr]);
        //std::cout << "diff: " << diff << std::endl;
        if (diff <= 0) {
            newDir = handleBS(bs);
            newTrans = newDir * (0.01f - diff);
            astroMov[curr]->update(glm::translate(glm::mat4(1), newTrans));
            
            angle1 = asin(-newDir.y);
            angle2 = acos(newDir.z);
            if (abs(angle1 - angle2) < 0.1 || abs(angle1 + angle2) < 0.1) angle = angle1;
            else angle = pi - angle1;
            
            //std::cout << "bs: " << faceDir[curr] << " " << diff << std::endl;
            astroRot[curr]->update(glm::rotate(glm::mat4(1), angle - glm::radians(faceDir[curr]), glm::vec3(1, 0, 0)));
            faceDir[curr] = glm::degrees(angle);
            velocity[curr] = newDir;
            
            //std::cout << angle << " " << faceDir[curr] << std::endl;
            world->draw(glm::mat4(1));
        }
    }
    
    // astronauts
    for (int i = 0; i < 12; ++i) {
        if (i == curr) {
            if (initialize) break;
            continue;
        }
        if (!isShown[i]) continue;
        
        diff = astroSpheres[i]->collide(astroSpheres[curr]);
        if (diff <= 0) {
            newDir = handleBS(astroSpheres[i]);
            newTrans = newDir * (0.01f - diff);
            astroMov[curr]->update(glm::translate(glm::mat4(1), newTrans));
            
            angle1 = asin(-newDir.y);
            angle2 = acos(newDir.z);
            if (abs(angle1 - angle2) < 0.1 || abs(angle1 + angle2) < 0.1) angle = angle1;
            else angle = pi - angle1;
            
            //std::cout << "bs2: " << faceDir[curr] << " " << diff << std::endl;
            astroRot[curr]->update(glm::rotate(glm::mat4(1), angle - glm::radians(faceDir[curr]), glm::vec3(1, 0, 0)));
            faceDir[curr] = glm::degrees(angle);
            velocity[curr] = newDir;
            
            //std::cout << angle << " " << faceDir[curr] << std::endl;
            world->draw(glm::mat4(1));
        }
    }
    
    //std::cout << faceDir[curr] << std::endl;
}

void Window::npcMove(int i) {
    GLfloat currTime = glfwGetTime();
    GLfloat stTime, endTime, psTime;
    srand((unsigned)time(NULL));
    GLfloat dt = currTime - startTime;
    
    //std::cout << dt << " " << astroCnt << " " << isShown[i] << " " << std::endl;
    
    // Appear or start moving
    if (dt >= spawnTime[i] && dt <= spawnTime[i] + 0.1) {
        // New astronaut appear if total num < 5
        if (!isShown[i]) {
            if (astroCnt < 5) {
                isShown[i] = true;
                astroSys->displayToggle(i, true);
                ++astroCnt;
                partSys[i]->reSpawn(true);
            }
            // Save the next appear time
            else {
                spawnTime[i] += (rand() % 50 + 50) / 10.f;
                despawnTime[i] = -1;
                pauseTime[i] = -1;
                
                return;
                //std::cout << i << " spawn " << spawnTime[i] << std::endl;
            }
        }
        // Decide pause or end next
        if (!isMove[i]) {
            isMove[i] = true;
            
            stTime = spawnTime[i];
            endTime = stTime + (rand() % 50 + 30) / 10.f;
            psTime = stTime + (rand() % 50 + 30) / 10.f;
            despawnTime[i] = endTime;
            pauseTime[i] = psTime;
        }

        //std::cout << dt << " start: " << stTime << " ps: " << psTime << " end: " << endTime << std::endl;
    }
    
    // Stop moving
    else if (dt >= pauseTime[i] && dt <= pauseTime[i] + 0.1) {
        if (isMove[i]) {
            isMove[i] = false;
            
            psTime = pauseTime[i];
            stTime = psTime + (rand() % 30 + 10) / 10.f;
            spawnTime[i] = stTime;
            despawnTime[i] = -1;
        }
        
        //std::cout << dt << " pause: " << psTime << " st: " << stTime << std::endl;
    }
    
    // Particles appear
    else if (dt >= despawnTime[i] && dt <= despawnTime[i] + 0.1) {
        if (isShown[i]) {
            partSys[i]->reSpawn(false);
            
            endTime = despawnTime[i];
            stTime = endTime + (rand() % 50 + 100) / 10.f;
            spawnTime[i] = stTime;
            pauseTime[i] = -1;
            
            isShown[i] = false;
        }
        
        //std::cout << dt << " end: " << endTime << " st: " << stTime << std::endl;
    }
    // Model disappear
    else if (dt >= despawnTime[i] + 1 && dt <= despawnTime[i] + 1.1) {
        if (isMove[i]) {
            --astroCnt;
            isMove[i] = false;
            astroSys->displayToggle(i, false);
        }
    }
}

