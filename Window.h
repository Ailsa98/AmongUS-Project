#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <vector>

#include "main.h"
#include "shader.h"
//#include "Skybox.h"
#include "Transform.h"
#include "Geometry.h"
#include "Sphere.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "Astronaut.h"
#include "Lobby.h"
#include "BoundingPlane.h"
#include "BoundingSphere.h"

class Window {
public:
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;

    // Objects to Render
    static Lobby * lobby;
    static std::vector<Astronaut*> astros;
    
    // Transforms
    static Transform * world;
    static Transform * astroSys;
    static Transform * lob;
    static Transform * camCtrl;
    static std::vector<Transform*> astroRot;
    static std::vector<Transform*> astroMov;
    
    // Trackball
    static bool leftPress;
    static glm::vec3 prevPos;
    
    // Render Control
    static bool useTex;
    static bool useToon;
    static int lightSrc;
    
    // Color
    static Material * black;
    static Material * blue;
    static Material * brown;
    static Material * cyan;
    static Material * darkGreen;
    static Material * lime;
    static Material * orange;
    static Material * pink;
    static Material * purple;
    static Material * red;
    static Material * white;
    static Material * yellow;
    static Material * silver;
    static std::vector<Material*> astroColors;
    
    // Light
    static PointLight * ptLight;
    static DirectionalLight * dirLight;
    
    // Projection matrix:
    static glm::mat4 projection;
    
    // Camera
    static Camera * camera;
    
    // Filepath
    static std::string directory;

    // Shader Program ID
    static GLuint phongShader;
    static GLuint basicShader;
    static GLuint skyboxShader;
    static GLuint refShader;
    
    // Movement handle
    static std::vector<GLfloat> faceDir;
    static std::vector<bool> isMove;
    static std::vector<glm::vec3> velocity;
    static int currAstro;
    static GLfloat userSpeed;
    
    // bounds
    static std::vector<BoundingSphere*> astroSpheres;
    static std::vector<BoundingPlane*> lobPlanes;
    static std::vector<BoundingSphere*> lobSpheres;
    static BoundingSphere* bsLeft, *bsRight;
    static BoundingPlane* top, *bottom, *left, *right, *ltCorner, *rtCorner;

    // Constructors and Destructors
    static bool initializeProgram();
    static bool initializeObjects();
    static void cleanUp();

    // Window functions
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    // Draw and Update functions
    static void idleCallback();
    static void displayCallback(GLFWwindow*);

    // Callbacks
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static glm::vec3 trackBallMapping(GLFWwindow* window, double xPos, double yPos);
    static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    // Textures
    static unsigned int loadTexture(std::string texLocation);
    
    // Collision
    static bool checkCollision(int curr);
    static void handleCollision(int curr, bool initialize = false);
    
    // Movement
    static void move(int curr);
    
};

#endif
