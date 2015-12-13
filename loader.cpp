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


#include "loader.h"

material::material(std::string name, float alpha, float ns, float ni, const glm::vec3& diff, const glm::vec3& amb, const glm::vec3& spec, int illum)
{
  this->name = name;
  this->alpha = alpha;
  this->ns = ns;
  this->ni = ni;
  this->diff = diff;
  this->amb = amb;
  this->spec = spec;
  this->illum = illum;
}

texcoord::texcoord(float u, float v)
{
  this->u = u;
  this->v = v;
}

Loader::Loader(const std::string filename)
{
  this->filename = filename;
  hasMaterial = false;
}

Loader::~Loader()
{
  for (unsigned int i = 0; i < lines.size(); i++)
  {
    delete lines[i];
  }

  for (unsigned int j = 0; j < faces.size(); j++)
  {
    delete faces[j];
  }

  for (unsigned int i = 0; i < materials.size(); i++)
  {
    delete materials[i];
  }

  for (unsigned int i = 0; i < tex_coordinates.size(); i++)
  {
    delete tex_coordinates[i];
  }

  lines.clear();
  vertices.clear();
  faces.clear();
  materials.clear();
  tex_coordinates.clear();
  normals.clear();
  indices.clear();


  for (std::vector<unsigned int>::const_iterator it = lists.begin(); it != lists.end(); it++)
  {
    glDeleteLists(*it, 1);
  }

}

void Loader::clean()
{
  for (unsigned int i = 0; i < lines.size(); i++)
  {
    delete lines[i];
  }

  for (unsigned int j = 0; j < faces.size(); j++)
  {
    delete faces[j];
  }

  for (unsigned int i = 0; i < materials.size(); i++)
  {
    delete materials[i];
  }

  for (unsigned int i = 0; i < tex_coordinates.size(); i++)
  {
    delete tex_coordinates[i];
  }

  vertices.clear();
  lines.clear();
  faces.clear();
  materials.clear();
  tex_coordinates.clear();
  indices.clear();
}

bool Loader::load()
{
  std::ifstream in(filename);

  if(!in.is_open())
  {
    std::cout << "Can't open file " << filename << std::endl;
    return false;
  }

  char buf[256];
  int curmat = 0;

  while (!in.eof())
  {
    in.getline(buf, 256);
    lines.push_back(new std::string(buf));
  }

  for (unsigned i = 0; i < lines.size(); i++)
  {
    if (lines[i]->c_str()[0] == '#')
    {
      continue;
    } else if(lines[i]->c_str()[0] == 'v' && lines[i]->c_str()[1] == ' ')
    {
      glm::vec3 points;
      sscanf(lines[i]->c_str(), "v %f %f %f", &points.x, &points.y, &points.z);
      vertices.push_back(glm::vec3(points.x, points.y, points.z));

    } else if(lines[i]->c_str()[0] == 'v' && lines[i]->c_str()[1] == 'n')
    {
      glm::vec3 points;
      sscanf(lines[i]->c_str(), "vn %f %f %f", &points.x, &points.y, &points.z);
      normals.push_back(glm::vec3(points.x, points.y, points.z));

    } else if(lines[i]->c_str()[0] == 'f' && lines[i]->c_str()[1] == ' ')
    {
      int fn, p1, p2, p3;
      if (lines[i]->find("//") != std::string::npos)
      {
          sscanf(lines[i]->c_str(), "f %d//%d %d//%d %d//%d", &p1, &fn, &p2, &fn, &p3, &fn);
          faces.push_back(new aface(fn, p1, p2, p3, 0, 0, 0, curmat));
      } else if (lines[i]->find("/") != std::string::npos){
          int t[3];
          sscanf(lines[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &p1, &t[0], &fn, &p2, &t[1], &fn, &p3, &t[2], &fn);
          faces.push_back(new aface(fn, p1, p2, p3, t[0], t[1], t[2], curmat));
      }
    } else if(lines[i]->c_str()[0] == 'u' && lines[i]->c_str()[1] == 's' && lines[i]->c_str()[2] == 'e')
    {
      char tmp[200];
      sscanf(lines[i]->c_str(), "usemtl %s", tmp);
      for (unsigned int i = 0; i < materials.size(); i++)
      {
          if(std::strcmp(materials[i]->name.c_str(), tmp) == 0)
          {
              curmat = i;
              break;
          }
      }
    } else if(lines[i]->c_str()[0] == 'm' && lines[i]->c_str()[1] == 't' && lines[i]->c_str()[2] == 'l')
    {
      char filen[100];
      sscanf(lines[i]->c_str(), "mtllib %s", filen);
      std::ifstream mtlin(filen);
      if (!mtlin.is_open())
      {
          std::cout << "couldn't open the material file called " << filen << std::endl;
      }
      hasMaterial = true;
      std::vector<std::string> mtl_lines;
      char c[100];
      while (!mtlin.eof())
      {
          mtlin.getline(c, 100);
          mtl_lines.push_back(c);
      }
      char name[100];
      char temp_tex_name[100];
      glm::vec3 diff, amb, spec;
      float ns = 0.0, ni = 0.0, alpha = 0.0;
      int illum = 0;
      bool isMat = false;
      std::strcpy(temp_tex_name, "\0");
      for (unsigned int i = 0; i < mtl_lines.size(); i++)
      {
          if (mtl_lines[i][0]=='#')
          {
              continue;
          }

          if (mtl_lines[i][0]=='n' && mtl_lines[i][1]=='e' && mtl_lines[i][2]=='w')
          {
              if (isMat)
              {
                  if (strcmp(temp_tex_name, "\0") != 0)
                  {
                      materials.push_back(new material(name, alpha, ns, ni, diff, amb, spec, illum));
                  } else {
                      materials.push_back(new material(name, alpha, ns, ni, diff, amb, spec, illum));
                  }
              }
              isMat = false;
              sscanf(mtl_lines[i].c_str(), "newmtl %s", name);
          } else if (mtl_lines[i][0]=='N' && mtl_lines[i][1]=='s')
          {
              sscanf(mtl_lines[i].c_str(), "Ns %f", &ns);
              isMat = true;
          } else if (mtl_lines[i][0]=='K' && mtl_lines[i][1]=='a')
          {
              sscanf(mtl_lines[i].c_str(), "Ka %f %f %f", &amb[0], &amb[1], &amb[2]);
              isMat = true;
          } else if (mtl_lines[i][0]=='K' && mtl_lines[i][1]=='d')
          {
              sscanf(mtl_lines[i].c_str(), "Kd %f %f %f", &diff[0], &diff[1], &diff[2]);
              isMat = true;
          } else if (mtl_lines[i][0]=='K' && mtl_lines[i][1]=='s')
          {
              sscanf(mtl_lines[i].c_str(), "Ks %f %f %f", &spec[0], &spec[1], &spec[2]);
              isMat = true;
          } else if (mtl_lines[i][0]=='N' && mtl_lines[i][1]=='i')
          {
              sscanf(mtl_lines[i].c_str(), "Ni %f", &ni);
              isMat = true;
          } else if (mtl_lines[i][0]=='d' && mtl_lines[i][1]==' ')
          {
              sscanf(mtl_lines[i].c_str(), "d %f", &alpha);
              isMat = true;
          } else if(mtl_lines[i][0]=='i' && mtl_lines[i][1]=='l')
          {
              sscanf(mtl_lines[i].c_str(), "illum %d", &illum);
              isMat = true;
          }
      }

      if (isMat)
      {
          if (strcmp(temp_tex_name, "\0") != 0)
          {
              materials.push_back(new material(name, alpha, ns, ni, diff, amb, spec, illum));
          } else {
              materials.push_back(new material(name, alpha, ns, ni, diff, amb, spec, illum));
          }
      }


    } else if(lines[i]->c_str()[0] == 'v' && lines[i]->c_str()[1] == 't' && lines[i]->c_str()[2] == ' ')
    {
      float u, v;
      sscanf(lines[i]->c_str(), "vt %f %f", &u, &v);
      tex_coordinates.push_back(new texcoord(u, 1-v));
    }

    if (materials.size() == 0)
    {
        hasMaterial = false;
    } else {
        hasMaterial = true;
    }

  }
  return true;
}

GLuint Loader::draw()
{
  model = glGenLists(1);
  glNewList(model, GL_COMPILE);
  int last = -1;
  for (unsigned int i = 0; i < faces.size(); i++)
  {
  if (last != faces[i]->mat && hasMaterial)
  {
      float diffuse [] =
      { materials[faces[i]->mat]->diff[0], materials[faces[i]->mat]->diff[1], materials[faces[i]->mat]->diff[2],materials[faces[i]->mat]->alpha };

      float specular [] =
      { materials[faces[i]->mat]->spec[0], materials[faces[i]->mat]->spec[1], materials[faces[i]->mat]->spec[2],materials[faces[i]->mat]->alpha };

      float ambient [] =
      { materials[faces[i]->mat]->amb[0], materials[faces[i]->mat]->amb[1], materials[faces[i]->mat]->amb[2],materials[faces[i]->mat]->alpha };

      float shininess = materials[faces[i]->mat]->ns;
      float intensity = materials[faces[i]->mat]->ni;

      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
      glMaterialf(GL_FRONT_AND_BACK, GL_INTENSITY, intensity);

      last = faces[i]->mat;
  }
  glBegin(GL_TRIANGLES);
    glScaled(5, 5, 5);

    glNormal3f(normals[faces[i]->facenum-1].x, normals[faces[i]->facenum-1].y, normals[faces[i]->facenum-1].z);
    glTexCoord2f(tex_coordinates[faces[i]->texcoord[0]-1]->u, tex_coordinates[faces[i]->texcoord[0]-1]->v);
    glVertex3f(vertices[faces[i]->itsfaces[0]-1].x, vertices[faces[i]->itsfaces[0]-1].y, vertices[faces[i]->itsfaces[0]-1].z);

    glTexCoord2f(tex_coordinates[faces[i]->texcoord[1]-1]->u, tex_coordinates[faces[i]->texcoord[1]-1]->v);
    glVertex3f(vertices[faces[i]->itsfaces[1]-1].x, vertices[faces[i]->itsfaces[1]-1].y, vertices[faces[i]->itsfaces[1]-1].z);

    glTexCoord2f(tex_coordinates[faces[i]->texcoord[2]-1]->u, tex_coordinates[faces[i]->texcoord[2]-1]->v);
    glVertex3f(vertices[faces[i]->itsfaces[2]-1].x, vertices[faces[i]->itsfaces[2]-1].y, vertices[faces[i]->itsfaces[2]-1].z);
  glEnd();
  }
  glEndList();
  lists.push_back(model);

  clean();

  std::cout << filename << " is loaded successfully to the engine." << std::endl;

  return model;
}
