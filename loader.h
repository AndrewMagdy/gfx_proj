#ifndef __LOADER__
#define __LOADER__

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

#include <cstring>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <glm/glm.hpp>


struct aface
{
    int facenum;
    int itsfaces[3];
    int texcoord[3];
    int mat;
    aface(int facen, int f1, int f2, int f3, int t1, int t2, int t3, int mat)
    {
        this->facenum = facen;
        itsfaces[0] = f1;
        itsfaces[1] = f2;
        itsfaces[2] = f3;

        texcoord[0] = t1;
        texcoord[1] = t2;
        texcoord[2] = t3;
        this->mat = mat;
    }
};

struct material
{
    std::string name;
    float alpha, ns, ni;
    glm::vec3 diff, amb, spec;
    int illum;
    material(std::string name, float alpha, float ns, float ni, const glm::vec3& diff, const glm::vec3& amb, const glm::vec3& spec, int illum);
};

struct texcoord
{
    float u, v;
    texcoord(float u, float v);
};

class Loader
{
public:
    Loader(const std::string filename);
    ~Loader();

    bool load();
    GLuint draw();
    void clean();

private:
    std::string filename;
    GLuint model;

    std::vector<std::string*> lines;
    std::vector<glm::vec3> vertices;
    std::vector<aface*> faces;
    std::vector<glm::vec3> normals;
    std::vector<material*> materials;
    std::vector<texcoord*> tex_coordinates;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lists;
    bool hasMaterial, hasNormal;
};

#endif
