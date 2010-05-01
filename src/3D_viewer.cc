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
}

void Viewer3D::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  trackball_.SetUpGlCamera();
   
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

void CaptureViewer::paintGL() {
  Viewer3D::paintGL();
 
  if (doc_) {
    // Draw better cameras.
    glBegin(GL_POINTS);
    glColor4d(0,1,0,1);
    glVertex3d(doc_->RigX(), doc_->RigY(), doc_->RigZ());
    for (int i = 0; i < 2; ++i) {
      Vector3d pos = doc_->CameraPosition(i);
      glColor4f(1. - i * .5, .7, .5 + i * .5, 1);
      glVertex3dv(&pos[0]);
    }
    glEnd();

    const Geometry &g = doc_->CaptureGeometry();
    glBegin(GL_LINES);
    for (int i = 0; i < g.triangles_.size(); i += 3) {
      for (int j = 0; j < 3; ++j) {
        glColor4f(.5,.7,1,1);
        int a = g.triangles_[i + j];
        int b = g.triangles_[i + (j+1)%3];
        glVertex4fv(&g.vertex_[4 * a]);
        glVertex4fv(&g.vertex_[4 * b]);
      }
    }
    glEnd();
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
    const Geometry &g = doc_->TheaterGeometry();
    glBegin(GL_LINES);
    for (int i = 0; i < g.triangles_.size(); i += 3) {
      for (int j = 0; j < 3; ++j) {
        glColor4f(1,.7,.5,1);
        int a = g.triangles_[i + j];
        int b = g.triangles_[i + (j+1)%3];
        glVertex4fv(&g.vertex_[4 * a]);
        glVertex4fv(&g.vertex_[4 * b]);
      }
    }
    glEnd();
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

