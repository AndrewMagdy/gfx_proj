Mac & Linux only supported


SETUP STEPS:
1- Search for tutorials to install CMAKE ...
2- Search for tutorials to install GLFW
{
  Mac
  {
    - Download from http://www.glfw.org/
    - Open Terminal paste --> sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
    - This will enable cmake in terminal ...
    - then cd to the Downloads/glfw-3.x ...
    - type cmake .  "including the dot ."
    - type make
    - type sudo make install --> this when finished will show u in the terminal the path in which installed glfw ..
  }
}

3- Go Where the GLMeshLoader repo folder u cloned is
4- You will find a file named 'Makefile' .. open it with any text editor
5- let CURDIR equals the current directory where you have your cloned GLMeshLoader directory

if you are using a Mac then change MACCURDIR -> example: 'MACCURDIR='XXX/GLMeshLoader--master'
if you are on a Linux machine change LINUXCURDIR -> example: 'LINUXCURDIR='XXX/GLMeshLoader--master'



6- Open terminal and cd to OpenGLMeshLoader folder
7- type make to compile
8- type ./program to run


LIMITATIONS:
-This Library is to load only .obj formats.
  
-The 3D Model must be triangulated (each face consists of only 3 vertices), make sure you checked Triangulate while exporting from blender.
  
-The Library loads vertices, normals and material colors only.
  
