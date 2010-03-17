#ifndef UI_TVR_3D_VIEWER_H_
#define UI_TVR_3D_VIEWER_H_

#include <QGLWidget>
#include <QImage>

#include "Eigen/Dense"

#include "document.h"


// TODO(pau): Add viewers for the sensor, and screen geometries.


class ViewerCamera {
 public:
  ViewerCamera();
  void SetScreenSize(int width, int height);
  void SetUpGlCamera();
  void MouseTranslate(float x1, float y1, float x2, float y2);
  void MouseRevolve(float x1, float y1, float x2, float y2);
  void MouseZoom(float dw);
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

 private:
  // Intrinsic parameters.
  float field_of_view_;
  float near_;
  float far_;
  float screen_width_;
  float screen_height_;
  
  // Extrinsic parameters.
  // The parameters define the transformation between the world and the camera
  // frames as
  //     cam_coords = Rotation(orientation_) * (world_coords - revolve_point_)
  //                + revolve_point_in_cam_coords_.
  Eigen::Vector3f revolve_point_;
  Eigen::Vector3f revolve_point_in_cam_coords_; // Implicitly defines the position
                                         // of the camera.
  Eigen::Quaternionf orientation_; // Orientation of the world axis w.r.t.
                                   // the camera axis.

  // Interaction parameters.
  float translation_speed_;
  float zoom_speed_;
};

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
  ViewerCamera viewer_camera_;
  QPoint lastPos_;
};


class CaptureViewer : public Viewer3D {
  Q_OBJECT
  
 public:
  CaptureViewer(QGLWidget *share, QWidget *parent);
  virtual ~CaptureViewer() {}
  void SetDocument(SpDocument *doc) {
    doc_ = doc;
  }
  void paintGL() {
    Viewer3D::paintGL();
   
    if (doc_) {
      glBegin(GL_LINES);
      for (int i = 0; i < doc_->capture_geometry_.triangles_.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {
          glColor4f(.5,.7,1,1);
          int a = doc_->capture_geometry_.triangles_[i + j];
          int b = doc_->capture_geometry_.triangles_[i + (j+1)%3];
          glVertex4fv(&doc_->capture_geometry_.vertex_[4 * a]);
          glVertex4fv(&doc_->capture_geometry_.vertex_[4 * b]);
        }
      }
      glEnd();
    }
  }
 private:
  SpDocument *doc_;
};

class TheaterViewer : public Viewer3D {
  Q_OBJECT
  
 public:
  TheaterViewer(QGLWidget *share, QWidget *parent);
  virtual ~TheaterViewer() {}
  void SetDocument(SpDocument *doc) {
    doc_ = doc;
  }
  void paintGL() {
    Viewer3D::paintGL();
   
    if (doc_) {
      glBegin(GL_LINES);
      for (int i = 0; i < doc_->theater_geometry_.triangles_.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {
          glColor4f(1,.7,.5,1);
          int a = doc_->theater_geometry_.triangles_[i + j];
          int b = doc_->theater_geometry_.triangles_[i + (j+1)%3];
          glVertex4fv(&doc_->theater_geometry_.vertex_[4 * a]);
          glVertex4fv(&doc_->theater_geometry_.vertex_[4 * b]);
        }
      }
      glEnd();
    }
  }
 private:
  SpDocument *doc_;
};

class GeometryViewer : public Viewer3D {
  Q_OBJECT
  
 public:
  GeometryViewer(QGLWidget *share, QWidget *parent);
  virtual ~GeometryViewer() {}
  void SetGeometry(Geometry *geo) {
    geo_ = geo;
  }
  void paintGL() {
    Viewer3D::paintGL();
   
    if (geo_) {
      for (int s = 0; s < 2; ++s) {
        Geometry *g = geo_ + s;
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
 private:
  Geometry *geo_;
};

#endif // UI_TVR_3D_VIEWER_H_
