#ifndef TRACKBALL_H_
#define TRACKBALL_H_

#include "Eigen/Dense"

// TODO(pau): add presets for top, front, left, ...

class Trackball {
 public:
  Trackball();
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
};

#endif // TRACKBALL_H_
