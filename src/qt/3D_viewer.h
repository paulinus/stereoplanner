#ifndef _3D_VIEWER_H_
#define _3D_VIEWER_H_

#include <QGLWidget>
#include <QImage>

#include "Eigen/Dense"

#include "trackball.h"
#include "document.h"


// A widget displaying a 3D scene.
class Viewer3D : public QGLWidget {
  Q_OBJECT
  
 public:
  Viewer3D(QGLWidget *share, QWidget *parent);
  virtual ~Viewer3D() {}

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

 public slots:

 protected:
  // Drawing.
  void initializeGL();
  virtual void paintGL();
  void resizeGL(int, int);

  // Mouse.
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent *);

 private:
  Trackball trackball_;
  QPoint lastPos_;
};


class CaptureViewer : public Viewer3D {
  Q_OBJECT
  
 public:
  CaptureViewer(QGLWidget *share, QWidget *parent);
  virtual ~CaptureViewer() {}
  void SetDocument(SpDocument *doc);
  void paintGL();

 private:
  SpDocument *doc_;
};

class TheaterViewer : public Viewer3D {
  Q_OBJECT
  
 public:
  TheaterViewer(QGLWidget *share, QWidget *parent);
  virtual ~TheaterViewer() {}
  void SetDocument(SpDocument *doc);
  void paintGL();

 private:
  SpDocument *doc_;
};

class GeometryViewer : public Viewer3D {
  Q_OBJECT
  
 public:
  GeometryViewer(QGLWidget *share, QWidget *parent);
  virtual ~GeometryViewer() {}
  void SetGeometry(const Geometry *geo);
  void paintGL();

 private:
  const Geometry *geo_;
};

#endif // _3D_VIEWER_H_
