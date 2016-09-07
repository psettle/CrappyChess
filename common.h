//these are constants and libraries that are used by many parts of the game
#include<iostream>
#include<thread>
#include<vector>
#include<map>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL.h>

#define PI 3.14159265358979323846f
#define PI_D 3.14159265358979323846

#define WHITE GL_TRUE
#define BLACK GL_FALSE