#ifndef __TEXTURE__
#define __TEXTURE__


#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLFW/glfw3.h>
#elif __linux__
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GLFW/glfw3.h>
#else
	#error "Unknowk Compiler"
#endif

#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h"


class Texture
{
public:
  static GLuint loadPngTexture(const char* filename);

private:
  Texture();
  ~Texture();
};


#endif
