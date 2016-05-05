#ifndef GLUTIL_H
#define GLUTIL_H

//WIN32 INCLUDES
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//BASIC UTIL INCLUDES
#include <string>
#include <sstream>

//OPENGL INCLUDES
#include <GL\glew.h> //NEEDS TO BE INCLUDED FIRST
#include <GL\GL.h>
#include <GL\GLU.h>
#include <GL\glext.h>
#include <GL\wglext.h>

//LINK TO LIBRARIES
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")

bool OutErrorMsg(const char* message);

#endif