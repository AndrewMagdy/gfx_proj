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
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>
#elif __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>
#else
#error "Unknown Compiler"
#endif

#include "Tic.h"
#include "mesh.h"
#include "texture.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;


int error = 0;
int mouseX, mouseY;
bool mouseleftdown = false;
float rot = 0.0f;
float rotb = 40.0f;

int GposX, GposZ;
bool selected = false;
int ** arr;

bool anim_cata = true;
// Meshes
Mesh* castle;
Mesh* catapult_0;
Mesh* catapult_1;

Board * b;
// Textures
GLuint texWood;
GLuint texStone;
GLuint texGrass;


GLdouble zNear = 0.1;
GLdouble zFar = 1000000;

const struct Camera {
  float	eyeX ;
  float	eyeY;
  float	eyeZ;
  float	centerX;
  float	centerY;
  float	centerZ;
  float	fov;
} defaultCamera = {20, 470, 70, 20, 0, 0, 70};
Camera camera = defaultCamera;

const struct Grid {
  int startX;
  int startY;
  int stepX;
  int stepY;
  int endX;
  int endY;
} grid = {-200, -200, 43, 43, 200, 200 };


int getLatestError() {
  error++;
  return error;
}

void initGL() {

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

  texGrass = Texture::loadPngTexture("Textures/grass.png");
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
    //glScalef(2, 2, 2);
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
    //glScalef(2, 2, 2);
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
  //glScalef(2, 2, 2);
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
    //glScalef(2, 2, 2);
    glBindTexture(GL_TEXTURE_2D, texWood);
    catapult_1->render();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
  }
}

void drawSmallGrid(float x, float y, int gridX , int gridZ){
  for (int i = 0; i < 3; ++i) {
    for(int j = 0; j < 3; ++j){
      glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
      if(arr[(gridX * 3) + j] [ (gridZ * 3) + i] == 1 ) {
        glBindTexture(GL_TEXTURE_2D, texStone);
      }
      else  if (arr[(gridX * 3) + j] [ (gridZ * 3) + i] == 2 )  {
        glBindTexture(GL_TEXTURE_2D, texWood);
      }
      else {
        glBindTexture(GL_TEXTURE_2D, texGrass);
      }

      glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
      glColor3f(0,1,0);
      glPushMatrix();
      glBegin(GL_QUADS);
      glNormal3f(0, 1, 0);	// Set quad normal direction.
      glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
      glVertex3f(x + (43 * i), 1, y + (43 * j));
      glTexCoord2f(5, 0);
      glVertex3f(x + 40 + (43 * i) , 1, y + (43 * j));
      glTexCoord2f(5, 5);
      glVertex3f(x + 40 + (43 * i), 1, y + 40 + (43 * j));
      glTexCoord2f(0, 5);
      glVertex3f(x + (43 * i), 1, y + 40 + (43 * j));
      glEnd();
      glPopMatrix();
    }
  }

}
void drawGrid () {
  for (int i = 0; i < 3; ++i) {
    for(int j = 0; j < 3; ++j){
      drawSmallGrid(-200 + (133 * i), -200 + (133 * j), j, i);
    }
  }

}
std::vector<GLdouble> GetOGLPos(double x, double y)
{
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;

  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetIntegerv( GL_VIEWPORT, viewport );

  winX = (float)x;
  winY = (float)viewport[3] - (float)y;
  glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

  gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

  GLdouble arr[] = {posX,posY,posZ};
  vector<GLdouble> res (arr, arr + sizeof(arr) / sizeof(arr[0]) );
  return res;
}



void RenderGround()
{
  glDisable(GL_LIGHTING);	// Disable lighting

  glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

  glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

  glBindTexture(GL_TEXTURE_2D, texGrass);	// Bind the ground texture

  glPushMatrix();
  glBegin(GL_QUADS);
  glNormal3f(0, 1, 0);	// Set quad normal direction.
  glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
  glVertex3f(-200, 0, -200);
  glTexCoord2f(5, 0);
  glVertex3f(200, 0, -200);
  glTexCoord2f(5, 5);
  glVertex3f(200, 0, 200);
  glTexCoord2f(0, 5);
  glVertex3f(-200, 0, 200);
  glEnd();
  glPopMatrix();

  glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

  glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
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
  RenderGround();
  drawGrid();

  //The Camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(camera.fov, 300 / 300, zNear, zFar);

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    vector<GLdouble> oglPos = GetOGLPos(xpos, ypos);
    if(oglPos[2] >= -200 && oglPos[2] <= 200 ) {
      if(oglPos[0] >=-200 && oglPos[0] <= 200) {
        GposX = ((oglPos[2] - grid.startX)/grid.stepX);
        GposZ = ((oglPos[0] - grid.startY)/grid.stepY);
        selected = true;
        b->move(Location(GposX / 3, GposZ / 3), Location(GposX % 3, GposZ % 3));
        arr = b->getBoard();

        bool gameOver = b->isGameOver();
        cout<<gameOver<<endl;
      }
    }
  }
}


void initEngine () {
  Player p("1", 'x');
  Player p2("2", 'o');
  b = new Board(p, p2);
  arr = b->getBoard();
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

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);

  initGL();
  initEngine();

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
