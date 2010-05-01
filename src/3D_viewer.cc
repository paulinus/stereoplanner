#include <QtGui>
#include <QtOpenGL>

#include "3D_viewer.h"


Viewer3D::Viewer3D(QGLWidget *share, QWidget *parent) :
    QGLWidget(0, share) {
  setWindowTitle("3D View");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QSize Viewer3D::minimumSizeHint() const {
  return QSize(50, 50);
}

QSize Viewer3D::sizeHint() const {
  return QSize(800, 400);
}

void Viewer3D::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 1);
  glPointSize(3);
  glShadeModel(GL_FLAT);

  // Set up GL_LIGHT1
  GLfloat LightAmbient[]= { 0.3f, 0.3f, 0.4f, 1.0f }; 			
  GLfloat LightDiffuse[]= { 1.0f, 1.0f, 0.9f, 1.0f };				
  GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };				
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
  glEnable(GL_LIGHT1);
}

void Viewer3D::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  trackball_.SetUpGlCamera();
   
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor4f(1,0,0,1); glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
  glColor4f(0,1,0,1); glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
  glColor4f(0,0,1,1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
  glEnd();
  
  glColor4f(1,1,1,1);
}

void Viewer3D::resizeGL(int width, int height) {
  trackball_.SetScreenSize(width, height);
  glViewport(0, 0, width, height);
}

void Viewer3D::mousePressEvent(QMouseEvent *event) {
  lastPos_ = event->pos();
}

void Viewer3D::mouseMoveEvent(QMouseEvent *event) {
  float x1 = lastPos_.x();
  float y1 = lastPos_.y();
  float x2 = event->pos().x();
  float y2 = event->pos().y();
  
  if(x1 == x2 && y1 == y2) {
    return;
  }
  
  if (event->buttons() & Qt::LeftButton) {
    trackball_.MouseRevolve(x1, y1, x2, y2);
  }
  
  if (event->buttons() & Qt::RightButton) {
    trackball_.MouseTranslate(x1, y1, x2, y2);
  } 
  
  lastPos_ = event->pos();
  updateGL();
}

void Viewer3D::wheelEvent(QWheelEvent *event) {
  trackball_.MouseZoom(event->delta());
  updateGL();
}


CaptureViewer::CaptureViewer(QGLWidget *share, QWidget *parent)
    : Viewer3D(share, parent), doc_(0) {
  setWindowTitle("Capture Viewer");
}

void CaptureViewer::SetDocument(SpDocument *doc) {
  doc_ = doc;
  if (doc_) {
    // Make connections.
    connect(doc_, SIGNAL(DocumentChanged()), this, SLOT(updateGL()));
  }
}

static Vector3f Eucliean(const float *h) {
  return Vector3f(h[0] / h[3], h[1] / h[3], h[2] / h[3]);
}

static void ComputeNormal(const float *a, const float *b, const float *c,
                          float *n) {
  Vector3f aa = Eucliean(a);
  Vector3f bb = Eucliean(b);
  Vector3f cc = Eucliean(c);
  Vector3f u = bb - aa;
  Vector3f v = cc - aa;
  Vector3f nn = u.cross(v).normalized();
  n[0] = nn[0];
  n[1] = nn[1];
  n[2] = nn[2];
}

void DrawGeometry(const Geometry &g) {
  glEnable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < g.triangles_.size(); i += 3) {
    const float *a = &g.vertex_[4 * g.triangles_[i + 0]];
    const float *b = &g.vertex_[4 * g.triangles_[i + 1]];
    const float *c = &g.vertex_[4 * g.triangles_[i + 2]];
    float normal[3];
    ComputeNormal(a, b, c, normal);
    glNormal3f(normal[0], normal[1], normal[2]);
    glColor4f(.5,.7,1,1);
    glVertex4fv(a);
    glVertex4fv(b);
    glVertex4fv(c);
  }
  glEnd();
}

void CaptureViewer::paintGL() {
  Viewer3D::paintGL();
 
  if (doc_) {
    // Draw better cameras.
    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    glColor4d(0,1,0,1);
    glVertex3d(doc_->RigX(), doc_->RigY(), doc_->RigZ());
    for (int i = 0; i < 2; ++i) {
      Vector3d pos = doc_->CameraPosition(i);
      glColor4f(1. - i * .5, .7, .5 + i * .5, 1);
      glVertex3dv(&pos[0]);
    }
    glEnd();

    DrawGeometry(doc_->CaptureGeometry());
  }
}


TheaterViewer::TheaterViewer(QGLWidget *share, QWidget *parent)
    : Viewer3D(share, parent), doc_(0) {
  setWindowTitle("Theater Viewer");
}

void TheaterViewer::SetDocument(SpDocument *doc) {
  doc_ = doc;
  if (doc_) {
    // Make connections.
    connect(doc_, SIGNAL(DocumentChanged()), this, SLOT(updateGL()));
  }
}

void TheaterViewer::paintGL() {
  Viewer3D::paintGL();
 
  if (doc_) {
    // Draw the screen.
    float w = doc_->ScreenWidth() / 2;
    float h = doc_->ScreenHeight() / 2;
    glBegin(GL_LINES);
    glColor3f(.7, .7, .7);
    glVertex3f(-w, -h, 0); glVertex3f(+w, -h, 0);
    glVertex3f(+w, -h, 0); glVertex3f(+w, +h, 0);
    glVertex3f(+w, +h, 0); glVertex3f(-w, +h, 0);
    glVertex3f(-w, +h, 0); glVertex3f(-w, -h, 0);
    glEnd();

    // Draw the observer.
    glBegin(GL_POINTS);
    glColor4d(0,1,0,1);
    glVertex3d(doc_->ObserverX(), doc_->ObserverY(), doc_->ObserverZ()); 
    for (int i = 0; i < 2; ++i) {
      Vector3d pos = doc_->EyePosition(i);
      glColor4f(1. - i * .5, .7, .5 + i * .5, 1);
      glVertex3dv(&pos[0]);
    }
    glEnd();


    // Draw geometry.
    DrawGeometry(doc_->TheaterGeometry());
  }
}


GeometryViewer::GeometryViewer(QGLWidget *share, QWidget *parent)
    : Viewer3D(share, parent), geo_(0) {
  setWindowTitle("Generic Geometry Viewer");
}

void GeometryViewer::SetGeometry(const Geometry *geo) {
  geo_ = geo;
}

void GeometryViewer::paintGL() {
  Viewer3D::paintGL();
 
  if (geo_) {
    for (int s = 0; s < 2; ++s) {
      const Geometry *g = geo_ + s;
      glBegin(GL_LINES);
      for (int i = 0; i < g->triangles_.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {
          glColor4f(.5 + s*.5,.7,1 - s*.5,1);
          int a = g->triangles_[i + j];
          int b = g->triangles_[i + (j+1)%3];
          glVertex4fv(&g->vertex_[4 * a]);
          glVertex4fv(&g->vertex_[4 * b]);
        }
      }
      glEnd();
    }
  }
}

