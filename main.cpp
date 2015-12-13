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



int error = 0;
float rot = 0.0f;
float rotb = 40.0f;

bool anim_cata = true;
// Meshes
Mesh* castle;
Mesh* catapult_0;
Mesh* catapult_1;
// Textures
GLuint texWood;
GLuint texStone;



const struct Camera {
	float	eyeX ;
	float	eyeY;
	float	eyeZ;
	float	centerX;
	float	centerY;
	float	centerZ;
	float	fov;
} defaultCamera = {20, 10, 20, 20, 0, 0, 56};
Camera camera = defaultCamera;


int getLatestError() {
	error++;
	return error;
}

void initGL() {

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glm::vec3 lightposition = glm::vec3(0, 500, 0);
	GLfloat lightpos[] = {lightposition.x, lightposition.y, lightposition.z, 1};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightpos);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	castle = new Mesh("./models/castle.obj");
	castle->startLoading();

	catapult_0 = new Mesh("models/catapult_0.obj");
	catapult_0->startLoading();

	catapult_1 = new Mesh("models/catapult_1.obj");
	catapult_1->startLoading();

	texWood = Texture::loadPngTexture("Textures/wood.jpg");

	texStone = Texture::loadPngTexture("Textures/stone.jpg");

}
void animateCatapult () {
	if (rotb <= 0) {
		anim_cata = false;
	}
	if (rot > 40) {
		glPushMatrix();
		glTranslatef(20, 0, 0);
		glRotatef(45, 0, 1, 0);
		glRotatef(rotb, 1, 0, 0);
		glScalef(2, 2, 2);
		glBindTexture(GL_TEXTURE_2D, texWood);
		catapult_1->render();
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
		rotb--;
	}
	else {
		glPushMatrix();
		glTranslatef(20, 0, 0);
		glRotatef(45, 0, 1, 0);
		glRotatef(rot, 1, 0, 0);
		glScalef(2, 2, 2);
		glBindTexture(GL_TEXTURE_2D, texWood);
		catapult_1->render();
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
		rot++;
	}
}
void drawCatapult () {
	glPushMatrix();
	glTranslatef(20, 0, 0);
	glRotatef(45, 0, 1, 0);
	glScalef(2, 2, 2);
	glBindTexture(GL_TEXTURE_2D, texWood);
	catapult_0->render();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	if (anim_cata) {
		animateCatapult ();
	}
	else {
		glPushMatrix();
		glTranslatef(20, 0, 0);
		glRotatef(45, 0, 1, 0);
		glScalef(2, 2, 2);
		glBindTexture(GL_TEXTURE_2D, texWood);
		catapult_1->render();
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texStone); //bind the texture to the next mesh rendered
	castle->render();
	glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
	glPopMatrix();

	drawCatapult();


	//The Camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera.fov, 300 / 300, 0.1f, 300.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.eyeX, camera.eyeY ,camera.eyeZ , camera.centerX, camera.centerY, camera.centerZ, 0.0f, 1.0f, 0.0f);

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

	if (glfwInit() == false)
	return getLatestError();
	glfwWindowHint(GLFW_SAMPLES, 2);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Basic", NULL, NULL);
	if (!window)
	{
		std::cout << "ERROR GLFWWINDOW NOT READY:\n" << getLatestError() << ".\n";
		return getLatestError();
	}

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	initGL();
	//Main Loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		render();
		glfwSwapBuffers(window);
	}

	// Clean Up
	delete castle;
	delete catapult_0;
	delete catapult_1;
	glDeleteTextures(1, &texWood);
	glDeleteTextures(1, &texStone);

	glfwTerminate();
	return 0;
}
