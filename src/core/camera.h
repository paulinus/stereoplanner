#ifndef __Camera_h__
#define __Camera_h__

#include "Eigen/Dense"

using namespace Eigen;


class Camera {
 public:
  Camera(float focal_length, float sensor_width, float sensor_height,
         float pp_x, float pp_y,
         const Vector3f &position, const Matrix3f &orientation) {
    focal_length_ = focal_length;
    sensor_width_ = sensor_width;
    sensor_height_ = sensor_height;
    //  (R|t) * (O|p) = I 
    //     R*O = I  ->   R = O^T
    //     Rp + t = 0  ->  t = -Rp
    R_ = orientation.transpose();
    t_ = - R_ * position;
    K_ << 2 * focal_length_ / sensor_width_, 0, pp_x,
          0, 2 * focal_length_ / sensor_height_, pp_y,
          0, 0, 1;
    P_ << K_ * R_, K_ * t_;
  }


  /////////////////////////////////////////////////
  // Projections

  /** \brief Projects a point into the image.
   *
   *  \param [out] u  The x coordinate of the projected point.
   *  \param [out] z  The y coordinate of the projected point.
   *  \param [out] zi  The depth of the point with respect to the camera.
   *  \param [in]  x  The x coordinate of the 3D point.
   *  \param [in]  y  The y coordinate of the 3D point.
   *  \param [in]  z  The z coordinate of the 3D point.
   *
   * This function assumes that P = K(R|t) with K(2,2) = 1.  See CheckK() doc.
   */
  void Project(float &u, float &v, float &zi,
               float x, float y, float z) const {
    Project(u, v, zi, x, y, z, 1);
  }

  void Project(float &u, float &v, float &zi,
               float x, float y, float z, float w) const {
    zi = P_(2, 0) * x + P_(2, 1) * y + P_(2, 2) * z + P_(2, 3) * w;
    u = (P_(0, 0) * x + P_(0, 1) * y + P_(0, 2) * z + P_(0, 3) * w) / zi;
    v = (P_(1, 0) * x + P_(1, 1) * y + P_(1, 2) * z + P_(1, 3) * w) / zi;
  }

  /** \brief The camera coordinates of a point given its world coordinates.
   *
   *  \param [out] xc  The x coordinate of in the camera frame.
   *  \param [out] yc  The y coordinate of in the camera frame.
   *  \param [out] zc  The z coordinate of in the camera frame.
   *  \param [in] x  The x coordinate of in the world frame.
   *  \param [in] y  The y coordinate of in the world frame.
   *  \param [in] z  The z coordinate of in the world frame.
   */
  void CameraCoordinates(float &xc, float &yc, float &zc,
                         float x, float y, float z ) const {
    xc = R_(0, 0) * x + R_(0, 1) * y + R_(0, 2) * z + t_(0);
    yc = R_(1, 0) * x + R_(1, 1) * y + R_(1, 2) * z + t_(1);
    zc = R_(2, 0) * x + R_(2, 1) * y + R_(2, 2) * z + t_(2);
  }

 private:
  Matrix3f K_;
  Matrix3f R_;
  Vector3f t_;

  Matrix<float, 3, 4> P_;

  float focal_length_;  // in mm
  float sensor_width_, sensor_height_;  // in mm
  float near_, far_;
};


class NearAndFarEstimator {
public:
  const Camera &c;
  float near,far;

  NearAndFarEstimator(const Camera &cam) : c(cam) {
    near = 1e20;
    far = 0;
  }

  void push(const float x, const float y, const float z) {
    float u, v, zi;
    c.Project(u, v, zi, x, y, z);
    if(zi < near) near = zi;
    if(far < zi) far = zi;
  }
};

#endif //__Camera_h__
