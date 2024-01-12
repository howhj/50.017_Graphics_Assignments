#ifndef MESH_H
#define MESH_H
#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include "Object3D.h"
#include "Triangle.h"
#include "vecmath.h"

//by default counterclockwise winding is front face
struct Trig
{
  Trig(){x[0]=0;x[1]=0;x[2]=0;}
  int & operator[](const int i) {return x[i];}
  int x[3];
  int texID[3];
};

class Mesh:public Object3D
{
public:
  Mesh(const char * filename, Material* m);
  std::vector<vec3>v;
  std::vector<Trig>t;
  std::vector<vec3>n;
  std::vector<vec2>texCoord;

  virtual bool intersect( const Ray& r , Hit& h , float tmin );

private:
  void compute_norm();
};

#endif
