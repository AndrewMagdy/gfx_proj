#ifndef __MESH__
#define __MESH__

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
#include <glm/glm.hpp>

#include "loader.h"

class Mesh
{
public:
  Mesh(const std::string filename);
  ~Mesh();

  void startLoading();
  void render();

private:
  std::string filename;
  GLuint model_id;
};

#endif
