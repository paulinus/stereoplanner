#include "geometry.h"



void ReadGeo(std::istream *input_stream, Geometry *g) {
  std::istream &fin = *input_stream;
  
  int num_vertices, num_faces;
  fin >> num_vertices >> num_faces;
  
  g->vertex_.resize(num_vertices * 4);
  g->normal_.resize(num_vertices * 3);
  g->triangles_.resize(num_faces * 3);
  
  const int buf_size = 10000;
  char buf[buf_size];  // Read anything remaining in the line including the '\n'.
  fin.getline(buf, buf_size - 1);
  
  fin.read((char *)&g->vertex_[0],
           sizeof(g->vertex_[0]) * g->vertex_.size());
  fin.read((char *)&g->normal_[0],
           sizeof(g->normal_[0]) * g->normal_.size());
  fin.read((char *)&g->triangles_[0],
           sizeof(g->triangles_[0]) * g->triangles_.size());
}

void ReadGeo(const char *filename, Geometry *g) {
  std::ifstream fin(filename);
  ReadGeo(&fin, g);
}



void ReadObj(std::istream *input_stream, Geometry *g) {
  std::istream &fin = *input_stream;
  
  g->vertex_.clear();
  g->triangles_.clear();
  
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


void ReadObj(const char *filename, Geometry *g) {
  std::ifstream fin(filename);
  ReadObj(&fin, g);
}

void ReadObjFromContent(const char *content, Geometry *g) {
  const std::string s(content);
  std::istringstream stream(s);
  ReadObj(&stream, g);
}



void ProjectGeometry(const Geometry &g, const Camera &c, Geometry *p) {
  p->vertex_.resize(g.vertex_.size());
  for (unsigned int i = 0; i < g.vertex_.size(); i += 4) {
    float u, v, zi;
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

void ScaleGeometry(const Geometry &g, float fx, float fy, float fz,
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

