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
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


int error = 0;
int mouseX, mouseY;
bool mouseleftdown = false;
float rot = 0.0f;
float rotb = 40.0f;
float t =0 ;

int delay = 0;

int GposX, GposZ;
bool selected = false;
int ** arr;
int ** win;


sf::SoundBuffer buffer;
sf::Sound sound;

bool anim_cata_0 = false;
bool anim_cata_1 = false;
bool animBall = false;

bool endG = true;
Player p("1", 'x');
Player p2("2", 'o');
// Meshes
Mesh* castle;
Mesh* catapult_0;
Mesh* catapult_1;
Mesh* o;
Mesh* x;

float y;

Board * b;
// Textures
GLuint texWood;
GLuint texStone;
GLuint texGrass;
GLuint skybox [6];

GLdouble zNear = 0.1;
GLdouble zFar = 1000000;

int zoxx = -1,zoxy = -1;


struct Camera {
  float	eyeX ;
  float	eyeY;
  float	eyeZ;
  float	centerX;
  float	centerY;
  float	centerZ;
  float	fov;
} defaultCamera = {-890, 340, 70, 10, 0, -20, 40};
//defaultCamera = {20, 1000, 70, 20, 0, 0, 40};
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
  glm::vec3 lightposition = glm::vec3(20, 500, 70);
  GLfloat lightpos[] = {lightposition.x, lightposition.y, lightposition.z, 1};
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightpos);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);

  castle = new Mesh("./models/castle.obj");
  castle->startLoading();

  catapult_0 = new Mesh("models/catapult_0.obj");
  catapult_0->startLoading();

  o = new Mesh("models/o.obj");
  o->startLoading();

  x = new Mesh("models/x.obj");
  x->startLoading();

  catapult_1 = new Mesh("models/catapult_1.obj");
  catapult_1->startLoading();

  texWood = Texture::loadPngTexture("Textures/wood.jpg");

  texStone = Texture::loadPngTexture("Textures/stone.jpg");

  texGrass = Texture::loadPngTexture("Textures/grass.png");

  skybox[0] = Texture::loadPngTexture("Textures/blood_sport512_front.jpg");
  skybox[1] = Texture::loadPngTexture("Textures/blood_sport512_left.jpg");
  skybox[2] = Texture::loadPngTexture("Textures/blood_sport512_back.jpg");
  skybox[3] = Texture::loadPngTexture("Textures/blood_sport512_right.jpg");
  skybox[4] = Texture::loadPngTexture("Textures/blood_sport512_top.jpg");
  skybox[5] = Texture::loadPngTexture("Textures/blood_sport512_top.jpg");
}

void animateCatapult (int i , int j,bool player) {

  GLuint texp;
  if(player)
  texp = texWood;
  else
  texp = texStone;

  if (rot > 40) {
    glPushMatrix();
    glTranslatef(-150 + (140 *i)  , 10, -150 + (140 *j));
    if(player)
    glRotatef(180, 0, 1, 0);
    glRotatef(rotb, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, texp);
    catapult_1->render();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    rotb--;
  }
  else {
    glPushMatrix();
    glTranslatef(-150 + (140 *i)  , 10, -150 + (140 *j));
    if(player)
    glRotatef(180, 0, 1, 0);
    glRotatef(rot, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, texp);
    catapult_1->render();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
    rot++;
  }
  if (rotb <= 0) {
    anim_cata_0 = false;
    anim_cata_1= false;
    rotb = 40.0f;
  }
}
void drawCatapult (int i , int j ,bool player) {
  GLuint texp;
  if(player)
  texp = texWood;
  else
  texp = texStone;

  glPushMatrix();
  glTranslatef(-150 + (140 *i)  , 10, -150 + (140 *j));
  if(player)
  glRotatef(180, 0, 1, 0);
  //glRotatef(45, 0, 1, 0);

  glBindTexture(GL_TEXTURE_2D, texp);
  catapult_0->render();
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();

  if (anim_cata_1 && anim_cata_0) {
    animateCatapult (i, j,player);
  }
  else {
    glPushMatrix();
    glTranslatef(-150 + (140 *i)  , 10, -150 + (140 *j));
    if(player)
    glRotatef(180, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texp);
    catapult_1->render();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
  }
}
void drawO (int j , int i) {
  glPushMatrix();
  //glRotatef(90,0,1,0);
  glTranslatef(-200 + (43 * i) + (3 * i/3) + 20, 0, -200 + (43 * j) + (3 * j/3)  + 20);
  glBindTexture(GL_TEXTURE_2D, texWood); //bind the texture to the next mesh rendered
  o->render();
  glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
  glPopMatrix();
}

void drawX (int j , int i) {
  glPushMatrix();
  //glRotatef(90,0,1,0);
  glTranslatef(-200 + (43 * i) + (3 * i/3) + 20, 0, -200 + (43 * j) + (3 * j/3)  + 20);
  glBindTexture(GL_TEXTURE_2D, texWood); //bind the texture to the next mesh rendered
  x->render();
  glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
  glPopMatrix();
}

void drawSmallGrid(float x, float y, int gridX , int gridZ){
  for (int i = 0; i < 3; ++i) {
    for(int j = 0; j < 3; ++j){
      glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
      if(arr[(gridX * 3) + j] [ (gridZ * 3) + i] == 1 ) {
        drawX((gridX * 3) + j,(gridZ * 3) + i);
      }
      else  if (arr[(gridX * 3) + j] [ (gridZ * 3) + i] == 2 )  {
        drawO ((gridX * 3) + j,(gridZ * 3) + i);
      }

      glBindTexture(GL_TEXTURE_2D, texGrass);
      glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
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
  glVertex3f(-600, 0, -600);
  glTexCoord2f(5, 0);
  glVertex3f(600, 0, -600);
  glTexCoord2f(5, 5);
  glVertex3f(600, 0, 600);
  glTexCoord2f(0, 5);
  glVertex3f(-600, 0, 600);
  glEnd();
  glPopMatrix();

  glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

  glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void output(float x, float z, string st)
{
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glColor3f(1,1,1);
	glRasterPos3f(x, 10, z);
	int len, i;
	len = (int) st.length();
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]);
	}
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glPopMatrix();
}
void displayText(){
  string x;
  x = "Player 1 won";
  output(0,0, x);
  if(b->isGameOver()){
    endG = false;
    if(b->isWinner(p)){
      x = "Player 1 won";
      output(0,0, x);
    }
    else if(b->isWinner(p2)){
      x = "Player 2 won";
      output(0,0, x);
    }
    else {
      x = "It is a tie";
      output(0,0, x);
    }
  }
}

void drawBall (int i, int j ,bool player) {
  glPushMatrix();
  if(!player) {

    y = 40 + ( (30 * t * sin(45)) -(t*t));
    float z = -215 + (140 * j) + (30 * t * cos(45)) ;


    if (z > 240) {
      z = 240;
    }
    glTranslatef(-168 + (140 *i) ,y ,z);

    glColor3f(1, 1, 9000);
    glutSolidSphere(10 , 25, 25);
    glPopMatrix();
  }
  else {

    y = 40 + ( (30 * t * sin(45)) -(t*t));
    float z = -225 + ((140 * (j+1)) - (30 * t * cos(45))) ;


    if (z < -260) {
      z = -260;
    }
    glTranslatef(-132 + (140 *i) ,y ,z);
    glColor3f(1, 1, 9000);
    glutSolidSphere(10 , 25, 25);
    glPopMatrix();

  }
}
void skyboxs () {
  // Store the current matrix
  glPushMatrix();

  // Enable/Disable features
  glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);

  // Just in case we set all vertices to white.
  glColor4f(1,1,1,1);

  // Render the front quad
  glBindTexture(GL_TEXTURE_2D, skybox[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex3f(  9000, -9000, -9000 );
  glTexCoord2f(1, 0); glVertex3f( -9000, -9000, -9000 );
  glTexCoord2f(1, 1); glVertex3f( -9000,  9000, -9000);
  glTexCoord2f(0, 1); glVertex3f(  9000,  9000, -9000);
  glEnd();

  // Render the left quad
  glBindTexture(GL_TEXTURE_2D, skybox[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex3f(  9000, -9000,  9000 );
  glTexCoord2f(1, 0); glVertex3f(  9000, -9000, -9000 );
  glTexCoord2f(1, 1); glVertex3f(  9000,  9000, -9000 );
  glTexCoord2f(0, 1); glVertex3f(  9000,  9000,  9000 );
  glEnd();

  // Render the back quad
  glBindTexture(GL_TEXTURE_2D, skybox[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex3f( -9000, -9000,  9000 );
  glTexCoord2f(1, 0); glVertex3f(  9000, -9000,  9000 );
  glTexCoord2f(1, 1); glVertex3f(  9000,  9000,  9000 );
  glTexCoord2f(0, 1); glVertex3f( -9000,  9000,  9000 );

  glEnd();

  // Render the right quad
  glBindTexture(GL_TEXTURE_2D, skybox[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex3f( -9000, -9000, -9000 );
  glTexCoord2f(1, 0); glVertex3f( -9000, -9000,  9000 );
  glTexCoord2f(1, 1); glVertex3f( -9000,  9000,  9000 );
  glTexCoord2f(0, 1); glVertex3f( -9000,  9000, -9000 );
  glEnd();

  // Render the top quad
  glBindTexture(GL_TEXTURE_2D, skybox[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 1); glVertex3f( -9000,  9000, -9000 );
  glTexCoord2f(0, 0); glVertex3f( -9000,  9000,  9000 );
  glTexCoord2f(1, 0); glVertex3f(  9000,  9000,  9000 );
  glTexCoord2f(1, 1); glVertex3f(  9000,  9000, -9000 );
  glEnd();

  // Render the bottom quad
  glBindTexture(GL_TEXTURE_2D, skybox[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0); glVertex3f( -9000, -9000, -9000 );
  glTexCoord2f(0, 1); glVertex3f( -9000, -9000,  9000 );
  glTexCoord2f(1, 1); glVertex3f(  9000, -9000,  9000 );
  glTexCoord2f(1, 0); glVertex3f(  9000, -9000, -9000 );
  glEnd();

  // Restore enable bits and matrix
  glPopAttrib();
  glPopMatrix();
}

void initSound(){
    if(buffer.loadFromFile("Untitled.ogg")){
        sound.setBuffer(buffer);
        sound.play();
    }
}


void render() {


  Player* zox = b->getTurn();
  zoxx = zox->locationToMove.getX();
  zoxy = zox->locationToMove.getY();

  cout<<zoxx<<" "<<zoxy<<endl;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(!endG){
    displayText();
  }
  else {

    skyboxs();

    if (y >= 20 && animBall){
      t+= 0.1;
    }
    else {
      if(delay <400){
        ++ delay;
      }
      else {
        t = 0;
        animBall = false;
        delay = 0;
      }
    }

    /*
    for (int i=0;i<3;++i){
      for (int j=0;j<3;++j){
      drawCatapult(i,j,0);
      drawBall(i,j,0);

      drawCatapult(i,j,1);
      drawBall(i,j,1);
    }
}*/


    glPushMatrix();
    glTranslatef(-30, 0, -350);
    glBindTexture(GL_TEXTURE_2D, texStone); //bind the texture to the next mesh rendered
    castle->render();
    glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
    glPopMatrix();
    glPushMatrix();

    glPushMatrix();
    glTranslatef(-200, 0, -350);
    glBindTexture(GL_TEXTURE_2D, texStone); //bind the texture to the next mesh rendered
    castle->render();
    glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
    glPopMatrix();
    glPushMatrix();

    glPushMatrix();
    glTranslatef(120, 0, -350);
    glBindTexture(GL_TEXTURE_2D, texStone); //bind the texture to the next mesh rendered
    castle->render();
    glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
    glPopMatrix();
    glPushMatrix();

    glTranslatef(-30, 0, 250);
    glBindTexture(GL_TEXTURE_2D, texStone); //bind the texture to the next mesh rendered
    castle->render();
    glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
    glPopMatrix();

    glTranslatef(-200, 0, 250);
    glBindTexture(GL_TEXTURE_2D, texStone); //bind the texture to the next mesh rendered
    castle->render();
    glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
    glPopMatrix();

    glTranslatef(120, 0, 250);
    glBindTexture(GL_TEXTURE_2D, texStone); //bind the texture to the next mesh rendered
    castle->render();
    glBindTexture(GL_TEXTURE_2D, 0); 	//unbind the texure to keep things clean
    glPopMatrix();
    int pla1 = 0 ,pla2 = 0 ;
    for (int i =0 ;i<3;++i) {
      for (int j =0 ;j<3;++j) {
        if(win[i][j] != 0) {
          drawCatapult(j,i,win[i][j]-1);
          drawBall(j,i,win[i][j]-1);
          if(win[i][j] == 1){
            pla1++;
          }
          else {
            pla2++;
          }
        }
        //cout<<win[i][j]<<" ";
      }
      //cout<<endl;
    }
    stringstream ss;
    ss<<"Player 1: "<<pla1<<" Player2: "<<pla2;
    output (-230 ,-210,ss.str());
    //cout<<endl<<endl;

    //drawCatapult(2,2,1);

    //drawCatapult(2,1,0);
    RenderGround();
    drawGrid();
  }
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
  if (action == GLFW_REPEAT || action == GLFW_PRESS)
  {
    switch (key)
    {
      case GLFW_KEY_W :
      camera.centerX += 10;
      break;
      case GLFW_KEY_S :
      camera.centerX-= 10;
      break;
      case GLFW_KEY_D :
      camera.centerZ += 10;
      break;
      case GLFW_KEY_A :
      camera.centerZ -= 10;
      break;
      case GLFW_KEY_X :
      camera.centerY+= 10;
      break;
      case GLFW_KEY_Z :
      camera.centerY-= 10;
      break;
      case GLFW_KEY_P :
      cout<<camera.eyeX<<" "<<camera.eyeY<<" "<<camera.eyeZ<<" "<<camera.centerX<<" "<<camera.centerY<<" "<<camera.centerZ<<endl;
      break;
      case GLFW_KEY_R :
      camera = {20, 1000, 70, 20, 0, 0, 40};
      break;
      case GLFW_KEY_T :
      camera = {-890, 340, 70, 10, 0, -20, 40};
      break;
      case GLFW_KEY_SPACE :
      if(!animBall)
      {
        animBall = 1;
        anim_cata_0 = true;
        anim_cata_1 = true;
        t = 0;
      }
      //anim_cata_1 = 1;
      break;
      case GLFW_KEY_UP :
      camera.eyeX += 10;
      break;
      case GLFW_KEY_DOWN :
      camera.eyeX -= 10;
      break;
      case GLFW_KEY_LEFT :
      camera.eyeY-= 10;
      break;
      case GLFW_KEY_RIGHT :
      camera.eyeY+= 10;
      break;
      case GLFW_KEY_K :
      camera.eyeZ -= 10;
      break;
      case GLFW_KEY_L :
      camera.eyeZ += 10;
      break;


      default:
      break;
    }
  }
  else if (action == GLFW_REPEAT)
  {
    switch (key)
    {



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
        endG = !b->isGameOver();
        win = b->winners();
      }
    }
  }
}


void initEngine () {
  b = new Board(p, p2);
  arr = b->getBoard();
  win = b->winners();
}
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
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
  initSound();
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
