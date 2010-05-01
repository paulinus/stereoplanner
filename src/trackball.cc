#include <QtGui>
#include <QtOpenGL>

#include "3D_viewer.h"


Trackball::Trackball() {  
  field_of_view_ = 60;
  near_ = 0.1;
  far_ = 1000;
  screen_width_ = 1;
  screen_height_ = 1;
  
  revolve_point_ << 0, 0, 0;
  revolve_point_in_cam_coords_ << 0, 0, -1;
  orientation_ = Eigen::Quaternionf(1, 0, 0, 0);
  
  translation_speed_ = 5;
  zoom_speed_ = 0.002;
}
  
void Trackball::SetScreenSize(int width, int height) {
  screen_width_ = width;
  screen_height_ = height;
}

void Trackball::SetUpGlCamera() {
  // Set intrinsic parameters.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(field_of_view_,
                 float(screen_width_) / float(screen_height_),
                 near_, far_);
  
  // Set extrinsic parametres.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // (1) Translate axis to revolve_point.
  glTranslatef(revolve_point_in_cam_coords_(0),
               revolve_point_in_cam_coords_(1),
               revolve_point_in_cam_coords_(2)); 
  // (2) Rotate.
  Eigen::AngleAxisf aa = orientation_;
  glRotatef(aa.angle() * 180. / M_PI, aa.axis()(0), aa.axis()(1), aa.axis()(2));
  // (3) Translate axis to world origin.
  glTranslatef(-revolve_point_(0), -revolve_point_(1), -revolve_point_(2));
}

void Trackball::MouseTranslate(float x1, float y1, float x2, float y2) {
  float dx = x2 - x1;
  float dy = y2 - y1;
  revolve_point_in_cam_coords_(0) += translation_speed_ * dx / screen_width_;
  revolve_point_in_cam_coords_(1) -= translation_speed_ * dy / screen_width_;
};

static Eigen::Vector3f LiftToTrackball(float x, float y,
                                    float width, float height) {
  float sphere_radius = std::min(width, height) / 2;

  // Normalize coordinates, and reverse y axis.
  x = (x - width / 2) / sphere_radius;
  y = - (y - height / 2) / sphere_radius;

  float r2 = x * x + y * y;
  float z;
  if (r2 < 0.5) {     
    z = sqrt(1 - r2);        // Lift to the sphere.
  } else {                    
    z = 1 / (2 * sqrt(r2));  // Lift to the hyperboloid.
  }
  return Eigen::Vector3f(x, y, z);
}

void Trackball::MouseRevolve(float x1, float y1, float x2, float y2) {
  if (x1 == x2 && y1 == y2) {
    return;
  }
  // Lift points to the trackball.
  Eigen::Vector3f p1 = LiftToTrackball(x1, y1, screen_width_, screen_height_);
  Eigen::Vector3f p2 = LiftToTrackball(x2, y2, screen_width_, screen_height_);

  // Compute rotation between the lifted vectors.
  Eigen::Quaternionf dq;
  dq.setFromTwoVectors(p1, p2);
  dq.normalize();
  
  // Apply the rotation.
  orientation_ = dq * orientation_;
  orientation_.normalize();
}

void Trackball::MouseZoom(float dw) {
  revolve_point_in_cam_coords_(2) += - zoom_speed_ * dw;
}

