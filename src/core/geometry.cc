#include "geometry.h"

void TriangleNormal(float *a, float *b, float *c, float *n) {
  float u[3] = {
    b[0] / b[3] - a[0] / a[3],
    b[1] / b[3] - a[1] / a[3],
    b[2] / b[3] - a[2] / a[3]
  };
  float v[3] = {
    c[0] / c[3] - a[0] / a[3],
    c[1] / c[3] - a[1] / a[3],
    c[2] / c[3] - a[2] / a[3]
  };
  float x = u[1] * v[2] - u[2] * v[1];
  float y = u[2] * v[0] - u[0] * v[2];
  float z = u[0] * v[1] - u[1] * v[0];
  float l = sqrt(x * x + y * y + z * z);
  n[0] = x / l;
  n[1] = y / l;
  n[2] = z / l;
}

void Geometry::ComputeNormals() {
  int num_vertices = vertex_.size() / 4;
  int num_triangles = triangles_.size() / 3;
  
  normal_.assign(3 * num_vertices, 0);
  std::vector<float> sumw(num_vertices, 0);
  
  for (int i = 0; i < num_triangles; ++i) {
    int ai = triangles_[3 * i];
    int bi = triangles_[3 * i + 1];
    int ci = triangles_[3 * i + 2];
    float *a = &vertex_[4 * ai];
    float *b = &vertex_[4 * bi];
    float *c = &vertex_[4 * ci];
    float n[3];
    TriangleNormal(a, b, c, n);
    for (int j = 0; j < 3; ++j) {
      normal_[3 * ai + 0] += n[0];
      normal_[3 * ai + 1] += n[1];
      normal_[3 * ai + 2] += n[2];
      sumw[ai] += 1;
      normal_[3 * bi + 0] += n[0];
      normal_[3 * bi + 1] += n[1];
      normal_[3 * bi + 2] += n[2];
      sumw[bi] += 1;
      normal_[3 * ci + 0] += n[0];
      normal_[3 * ci + 1] += n[1];
      normal_[3 * ci + 2] += n[2];
      sumw[ci] += 1;
    }
  }
  for (int i = 0; i < num_vertices; ++i) {
    normal_[3 * i + 0] /= sumw[i];
    normal_[3 * i + 1] /= sumw[i];
    normal_[3 * i + 2] /= sumw[i];
  }
}


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

void MergeGeometries(const Geometry &a, const Geometry &b, Geometry *res) {
  res->vertex_.insert(res->vertex_.end(), a.vertex_.begin(), a.vertex_.end());
  res->vertex_.insert(res->vertex_.end(), b.vertex_.begin(), b.vertex_.end());
  res->normal_.insert(res->normal_.end(), a.normal_.begin(), a.normal_.end());
  res->normal_.insert(res->normal_.end(), b.normal_.begin(), b.normal_.end());

  res->triangles_.reserve(a.triangles_.size() + b.triangles_.size());
  res->triangles_.insert(res->triangles_.end(), a.triangles_.begin(), a.triangles_.end());
  for (unsigned int i = 0; i < b.triangles_.size(); ++i) {
    res->triangles_.push_back(b.triangles_[i] + a.vertex_.size());
  }

  res->lines_.reserve(a.lines_.size() + b.lines_.size());
  res->lines_.insert(res->lines_.end(), a.lines_.begin(), a.lines_.end());
  for (unsigned int i = 0; i < b.lines_.size(); ++i) {
    res->lines_.push_back(b.lines_[i] + a.vertex_.size());
  }
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
  p->lines_ = g.lines_;
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
  p->lines_ = g.lines_;
}

