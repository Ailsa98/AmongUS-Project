#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to Render
Lobby * Window::lobby;
std::vector<Astronaut*> Window::astros = std::vector<Astronaut*>(12);

// Transforms
Transform * Window::world;
Transform * Window::astroSys;
Transform * Window::camCtrl;
Transform * Window::lob;
std::vector<Transform*> Window::astroRot = std::vector<Transform*>(12);
std::vector<Transform*> Window::astroMov = std::vector<Transform*>(12);

// Movement
float Window::x = 0;

// Trackball
bool Window::leftPress = false;
glm::vec3 Window::prevPos;

// Rendel control
bool Window::useTex = true;
bool Window::useToon = true;
int Window::lightSrc = 0;

// Default filepath
std::string Window::directory = "/Users/ailsafan/Desktop/demo/demo3/obj/";

// Lights
PointLight * Window::ptLight;
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
GLuint Window::skyboxShader;
GLuint Window::basicShader;
GLuint Window::refShader;

// Movement handle
std::vector<GLfloat> Window::faceDir = std::vector<GLfloat>(12, 0);
std::vector<bool> Window::isMove = std::vector<bool>(12, 0);

bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    phongShader = LoadShaders("shaders/phongShader.vert", "shaders/phongShader.frag");
    skyboxShader = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    //basicShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    refShader = LoadShaders("shaders/reflection.vert", "shaders/reflection.frag");

    // Check the shader program.
    if (!phongShader || !skyboxShader || !refShader) {
        std::cerr << "Failed to initialize shader programs" << std::endl;
        return false;
    }
    
    /*
    if (!basicShader) {
        std::cerr << "Failed to initialize basic shader program" << std::endl;
        return false;
    }
     */
    
    return true;
}

bool Window::initializeObjects() {
    // Initialize point lights
    ptLight = new PointLight(glm::vec3(1.f), glm::vec3(100, 50, 0), 0.01);
    dirLight = new DirectionalLight(glm::vec3(1.f), glm::vec3(-100, 200, 0));
    currLight = dirLight;
    
    // Initialize cameras
    camera = new Camera(glm::vec3(50, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camCtrl = new Transform(glm::mat4(1));
    camCtrl->addChild(camera);
    
    // Initialize objects
    glm::mat4 model = glm::mat4(1);
    lobby = new Lobby(directory);
    lobby->setColor(silver);
    lobby->setColor(white);
    lobby->setTexture(loadTexture(directory + "amongus_lobby.png"));
    lobby->useTex(useToon);
    //lobby->useToon(0);
    
    model = glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 0, 1));
    // 0.35 -> attach the lower floor
    model = glm::translate(model, glm::vec3(0, 1, 0));
    astroSys = new Transform(model);
    
    // Initialize the system
    for (int i = 0; i < 12; ++i) {
        astroRot[i] = new Transform(glm::mat4(1));
        astroMov[i] = new Transform(glm::mat4(1));
        astros[i] = new Astronaut(directory);
        astros[i]->setColor(astroColors[i]);
        astros[i]->useToon(useToon);
        astroMov[i]->addChild(astros[i]);
        astroRot[i]->addChild(astroMov[i]);
        astroSys->addChild(astroRot[i]);
    }
    
    //model = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0));
    
    // Randomize the initial direction
    srand((unsigned)time(NULL));
    for (int i = 0; i < 12; ++i) {
        if (i == 0) {
            faceDir[i] = rand() % 4 * 90;
        }
        else {
            faceDir[i] = rand() % 360;
        }
    }

    std::cout << faceDir[0] << std::endl;
    
    GLfloat x, y;
    
    
    // Randomize the initial position
    // 14 - - -14
    //      |
    //     -15
    srand((unsigned)time(NULL));
    for (int i = 0; i < 12; ++i) {
        astroMov[i]->update(glm::rotate(glm::mat4(1), glm::radians(faceDir[i]), glm::vec3(0, 1, 0)));
        x = rand() % 1600;
        x = x / 100;
        y = rand() % 2800;
        y = y / 100 - 14;
        astroRot[i]->update(glm::translate(glm::mat4(1), glm::vec3(x, 0, y)));
    }
    
    // Build relationships
    world = new Transform(glm::mat4(1));
    
    model = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0, 0, 1));
    model = glm::translate(model, glm::vec3(0, 0, -5));
    model = glm::scale(model, glm::vec3(1.1));
    lob = new Transform(model);
    lob->addChild(lobby);
    
    world->addChild(currLight);
    world->addChild(camera);
    world->addChild(astroSys);
    world->addChild(lob);
    
    //astros[0]->getBS()->printPos = true;
    
    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete world;

    // Delete the shader program.
    glDeleteProgram(phongShader);
    glDeleteProgram(skyboxShader);
    //glDeleteProgram(basicShader);
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

}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the objects
    //ptLight->draw(phongShader, glm::mat4(1));
    
    glUseProgram(phongShader);
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(phongShader, "view"), 1, GL_FALSE, glm::value_ptr(camera->newView));
    glUniform3fv(glGetUniformLocation(phongShader, "viewPos"), 1, glm::value_ptr(camera->newEyePos));

    world->draw(phongShader, glm::mat4(1));
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
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
                
            case GLFW_KEY_UP:
                if (faceDir[0] == 0) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 180) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 270) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(0, 1, 0)));
                }
                faceDir[0] = 90;
                astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(-0.2, 0, 0)));
                world->draw(phongShader, glm::mat4(1));
                //astros[0]->getBS()->printPos();
                
                if (checkCollision(0)) {
                    astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(0.2, 0, 0)));
                }
                break;
                
            case GLFW_KEY_DOWN:
                if (faceDir[0] == 0) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 180) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 90) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(0, 1, 0)));
                }
                faceDir[0] = 270;
                astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(0.2, 0, 0)));
                world->draw(phongShader, glm::mat4(1));
                //astros[0]->getBS()->printPos();
                
                if (checkCollision(0)) {
                    astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(-0.2, 0, 0)));
                }
                break;
                
            case GLFW_KEY_LEFT:
                if (faceDir[0] == 0) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 90) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 270) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 1, 0)));
                }
                faceDir[0] = 180;
                astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0.2)));
                world->draw(phongShader, glm::mat4(1));
                //astros[0]->getBS()->printPos();
                
                if (checkCollision(0)) {
                    astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, -0.2)));
                }
                break;
                
            case GLFW_KEY_RIGHT:
                if (faceDir[0] == 180) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(180.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 90) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(-90.f), glm::vec3(0, 1, 0)));
                }
                else if (faceDir[0] == 270) {
                    astroMov[0]->update(glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(0, 1, 0)));
                }
                faceDir[0] = 0;
                astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, -0.2)));
                world->draw(phongShader, glm::mat4(1));
                //astros[0]->getBS()->printPos();
                
                if (checkCollision(0)) {
                    astroRot[0]->update(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0.2)));
                }
                break;
                
                
                
            default:
                break;
        }
    }
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {

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

bool Window::checkCollision(int curr){
    for (auto bp : *lobby->getBP()) {
        if (bp->isCollide(astros[curr]->getBS())) {
            return true;
        }
    }
    for (auto bs : *lobby->getBS()) {
        if (bs->isCollide(astros[curr]->getBS())) {
            return true;
        }
    }
    /*
    for (int i = 0; i < 12; ++i) {
        if (i == curr)  continue;
        if (astros[i]->getBS()->isCollide(astros[curr]->getBS())) {
            return true;
        }
    }
     */
    return false;
}
