#include "geometry.h"

void ReadObj(const char *filename, Geometry *g) {
  g->vertex_.clear();
  g->triangles_.clear();

  std::ifstream fin(filename);
  while (fin.good()) {
    // Process 1 line.
    char c;
    fin >> c;
    if (c == 'v') {
      float x,y,z;
      fin >> x >> y >> z;
      g->vertex_.push_back(x);
      g->vertex_.push_back(y);
      g->vertex_.push_back(z);
      g->vertex_.push_back(1);
    } else if (c == 'f') {
      // Only triangular faces are accepted.
      int a, b, c;
      fin >> a >> b >> c;
      g->triangles_.push_back(a - 1);
      g->triangles_.push_back(b - 1);
      g->triangles_.push_back(c - 1);
    } else {
      char buf[3000];
      fin.getline(buf, 3000 - 1);
    } 
  }
}

void ProjectGeometry(const Geometry &g, const Camera &c, Geometry *p) {
  p->vertex_.resize(g.vertex_.size());
  for (unsigned int i = 0; i < g.vertex_.size(); i += 4) {
    double u, v, zi;
    c.Project(u,v,zi, g.vertex_[i + 0],
                      g.vertex_[i + 1],
                      g.vertex_[i + 2],
                      g.vertex_[i + 3]);
    p->vertex_[i + 0] = u;
    p->vertex_[i + 1] = v;
    p->vertex_[i + 2] = 0;
    p->vertex_[i + 3] = 1;
  }
  p->triangles_ = g.triangles_;
}

void ScaleGeometry(const Geometry &g, double fx, double fy, double fz,
                   Geometry *p) {
  p->vertex_.resize(g.vertex_.size());
  for (unsigned int i = 0; i < g.vertex_.size(); i += 4) {
    p->vertex_[i + 0] = fx * g.vertex_[i + 0];
    p->vertex_[i + 1] = fy * g.vertex_[i + 1];
    p->vertex_[i + 2] = fz * g.vertex_[i + 2];
    p->vertex_[i + 3] = g.vertex_[i + 3];
  }
  p->triangles_ = g.triangles_;
}

