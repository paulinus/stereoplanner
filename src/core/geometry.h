#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <fstream>
#include "Eigen/Dense"

#include "camera.h"


// TODO(pau) afegir normals a la geometria.

class Geometry {
 public:
  std::vector<float> vertex_;  // 4 floats per vertex.
  std::vector<float> normal_;  // 3 floats per vertex.
  std::vector<unsigned short> triangles_; // 3 ints per triangle.
};

void ReadGeo(std::istream *input_stream, Geometry *g);
void ReadGeo(const char *filename, Geometry *g);

void ReadObj(const char *filename, Geometry *g);
void ReadObjFromContent(const char *content, Geometry *g);




inline Geometry CubeGeometry() {
  Geometry g;
  float v[] = {-1,-1,-1,1,
                1,-1,-1,1,
                1, 1,-1,1,
               -1, 1,-1,1,
               -1,-1, 1,1,
                1,-1, 1,1,
                1, 1, 1,1,
               -1, 1, 1,1};
  g.vertex_.insert(g.vertex_.begin(), v, v + sizeof(v)/sizeof(v[0]));
  short int t[] = {0,1,2,
             2,3,0,
             0,1,5,
             5,4,0,
             1,2,6,
             6,5,1,
             2,3,7,
             7,6,2,
             3,0,4,
             4,7,3,
             4,5,6,
             6,7,4};
  g.triangles_.insert(g.triangles_.begin(), t, t + sizeof(t)/sizeof(t[0]));
  return g;
}


void ProjectGeometry(const Geometry &g, const Camera &c, Geometry *p);
void ScaleGeometry(const Geometry &g, float fx, float fy, float fz,
                   Geometry *p);


#endif // GEOMETRY_H_
