#ifndef SHADER_HPP
#define SHADER_HPP

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

#endif
