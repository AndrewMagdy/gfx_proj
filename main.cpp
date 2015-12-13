//////////////////////////// MIT LICENSE /////////////////////////////////////////
// Copyright(c) 2015 Omar Charif Fathi                                    			//
//                                                                              //
// Permission is hereby granted, free of charge, to any person obtaining a copy //
// of this software and associated documentation files (the "Software"), to deal//
// in the Software without restriction, including without limitation the rights //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell    //
// copies of the Software, and to permit persons to whom the Software is        //
// furnished to do so, subject to the following conditions:                     //
//                                                                              //
//                                                                              //
// The above copyright notice and this permission notice shall be included in   //
// all copies or substantial portions of the Software.                          //
//                                                                              //
//                                                                              //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,//
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN    //
// THE SOFTWARE.                                                                //
//////////////////////////////////////////////////////////////////////////////////


#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLFW/glfw3.h>
#elif __linux__
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GLFW/glfw3.h>
#else
	#error "Unknown Compiler"
#endif


#include <iostream>
#include <vector>
#include <string>

#include "mesh.h"
#include "texture.h"


//////////////////////////// GLOBAL VARIABLES //////////////////////////////////
int error = 0;																																//
float rot = 0.0f;																															//
Mesh* mesh;
Mesh* second;																																				//
GLuint texture_id_0;
GLuint texture_id_1;
																													//
////////////////////////////////////////////////////////////////////////////////

int getLatestError()
{
	error++;
	return error;
}

void initGL()
{
	//////////////////////////// SETUP OPENGL ENUMERATIONS ///////////////////////
	glClearColor(0.2, 0.2, 0.2, 1.0);																						//
	glEnable(GL_LIGHTING);																											//
  glEnable(GL_LIGHT0);																												//
	glm::vec3 lightposition = glm::vec3(0, 500, 0);															//
  GLfloat lightpos[] = {lightposition.x, lightposition.y, lightposition.z, 1};//
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightpos);													//
  glShadeModel(GL_SMOOTH);																										//
  glEnable(GL_MULTISAMPLE);																										//
	glEnable(GL_DEPTH_TEST);																										//
	//////////////////////////////////////////////////////////////////////////////



	//////////////////////////// INITIALIZE NEW MESHES ///////////////////////////
  mesh = new Mesh("./castle.obj");																						//
  mesh->startLoading();		

    second = new Mesh("./untitled.obj");																						//
  second->startLoading();																											//
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////// LOADING PNG TEXTURES ////////////////////////////
  texture_id_0 = Texture::loadPngTexture("wood.jpg");

  texture_id_1 = Texture::loadPngTexture("stone.jpg");											//
	//////////////////////////////////////////////////////////////////////////////
}

void render()
{
	//////////////////////////// RENDER SCENE HERE ///////////////////////////////
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);													//
  glMatrixMode(GL_PROJECTION);																								//
  glLoadIdentity();																														//
  gluPerspective(60.0f, 640.0/480.0, 0.001, 1000.0);													//
  glMatrixMode(GL_MODELVIEW);																									//
  glLoadIdentity();																														//
  gluLookAt(0, 0, 10, 0, 0, -2, 0, 1, 0);																			//
  rot++;																																			//
																																							//
	glPushMatrix();																															//
  	glRotatef(rot, 0, 1, 0);																									//
		//bind the texture to the next mesh rendered															//
		glBindTexture(GL_TEXTURE_2D, texture_id_1);																//
		mesh->render();																														//
		//unbind the texure to keep things clean																	//
		glBindTexture(GL_TEXTURE_2D, 0);																					//
	glPopMatrix();


		glPushMatrix();	
		glRotatef(45, 0, 1, 0);																														//
  		glRotatef(rot, 1, 0, 0);
																										//
		//bind the texture to the next mesh rendered															//
		glBindTexture(GL_TEXTURE_2D, texture_id_0);																//
		//second->render();																														//
		//unbind the texure to keep things clean																	//
		glBindTexture(GL_TEXTURE_2D, 0);																					//
	glPopMatrix();																																//
	//////////////////////////////////////////////////////////////////////////////
}

void key_callback(GLFWwindow* window, int key, int scanccode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE:
			glClearColor(0.0, 0.0, 1.0, 1.0);
			break;

		default:
			break;
		}
	}
	else if (action == GLFW_REPEAT)
	{
		switch (key)
		{

		case GLFW_KEY_SPACE:
			glClearColor(1.0, 0.0, 0.0, 1.0);
			break;

		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;

		default:
			break;
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//can also get this with glfwGetCursorPos(window, &xpos, &ypos);
	//std::cout << "[" << xpos << ", " << ypos << "]\n";
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;

		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;

		case GLFW_MOUSE_BUTTON_LEFT:
			break;

		default:
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	///////////////////////// GET GLFW READY AND CHECK FOR WIERD THINGS //////////
	if (glfwInit() == false)																										//
		return getLatestError();																									//
																																							//
	glfwWindowHint(GLFW_SAMPLES, 2);																						//
	GLFWwindow* window = glfwCreateWindow(640, 480, "Basic", NULL, NULL);				//
	if (!window)																																//
  {																																						//
    std::cout << "ERROR GLFWWINDOW NOT READY:\n" << getLatestError() << ".\n";//
		return getLatestError();																									//
  }																																						//
	//////////////////////////////////////////////////////////////////////////////



	//////////////////////////// SET CALLBACKS AND MAKE GL CONTEXT ///////////////
	glfwSetCursorPosCallback(window, cursor_position_callback);									//
	glfwSetMouseButtonCallback(window, mouse_button_callback);									//
	glfwSetKeyCallback(window, key_callback);																		//
	glfwMakeContextCurrent(window);																							//
	//////////////////////////////////////////////////////////////////////////////



	//////////////////////////// INITIALIZE OPENGL ///////////////////////////////
	initGL();																																		//
	//////////////////////////////////////////////////////////////////////////////



	//////////////////////////// START REDNDERING LOOP ///////////////////////////
	while (!glfwWindowShouldClose(window))																			//
	{																																						//
		glfwPollEvents();																													//
		render();																																	//
    glfwSwapBuffers(window);																									//
	}																																						//
	//////////////////////////////////////////////////////////////////////////////



	//////////////////////////// BE PRO AND LEAVE NO POINTERS LEFT BEHIND ////////
  delete mesh;																																//
	glDeleteTextures(1, &texture_id_0);																					//
	//////////////////////////////////////////////////////////////////////////////


	glfwTerminate();
	return 0;
}
