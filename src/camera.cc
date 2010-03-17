#include <algorithm>

#include <vnl/vnl_double_3x4.h>
#include <vnl/vnl_double_3x3.h>
#include <vnl/vnl_double_3.h>
#include <vnl/vnl_double_4.h>
#include <vnl/vnl_inverse.h>
#include <vnl/vnl_transpose.h>
#include <vnl/vnl_identity_3x3.h>

#include "core/real.h"
#include "core/camera.h"


void Camera::SetP(const vnl_double_3x4 &P_) {
  P = P_;
  KRt_from_P(K,R,t, P);

  SetKRt(K,R,t);
}

void Camera::SetKRt(const vnl_double_3x3 &K_,
                     const vnl_double_3x3 &R_,
                     const vnl_double_3 &t_) {
  K = K_;
  R = R_;
  t = t_;

  assert(CheckK());

  P = K * Rt();

  UpdateKR1();
  UpdateOpticalCenter();
}

bool Camera::CheckK(const double tol) {
  if (   fabs(K(1,0) - 0)<tol
      && fabs(K(2,0) - 0)<tol
      && fabs(K(2,1) - 0)<tol
      && fabs(K(2,2) - 1)<tol) {
    K(1,0) = 0;
    K(2,0) = 0;
    K(2,1) = 0;
    K(2,2) = 1;
    return true;
  } else {
    return false;
  }
}

vnl_double_3x4 Camera::Rt() {
  vnl_double_3x4 Rt;
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
      Rt(i,j) = R(i,j);
  for(int i=0;i<3;i++)
    Rt(i,3) = t(i);
  return Rt;
}

void Camera::UpdateKR1() {
  vnl_double_3x3 KR;
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
      KR(i,j)=P(i,j);
  KR1 = vnl_inverse(KR);
}

void Camera::UpdateOpticalCenter() {
  ocenter = - R.transpose() *t;
}

void Camera::ImageResize(int neww, int newh) {
  const double ratiow = double(neww) / width;
  const double ratioh = double(newh) / height;

  // from central pixel coordinates to corner pixel coordinates
  double h1[] = {1, 0, .5,
                 0, 1, .5,
                 0, 0,  1};
  // from the old size to the new size
  double h2[] = {ratiow,     0, 0,
                      0,ratioh, 0,
                      0,     0, 1};
  // from corner pixel coordinates to central pixel coordinates
  double h3[] = {1, 0, -.5,
                 0, 1, -.5,
                 0, 0,  1};

  vnl_double_3x3 H1(h1), H2(h2), H3(h3);

  K = H3 * H2 * H1 * K;

  width = neww;
  height = newh;
  SetKRt(K, R, t);
}

void Camera::IncludeBetweenNearAndFar(const REAL x,
                                      const REAL y,
                                      const REAL z) {
  REAL u, v, zi;
  Project(u, v, zi, x, y, z);
  if (zi < near) near = zi;
  if (far < zi) far = zi;
}

void Camera::SetNearAndFarFromBoundingBox(BoundingBox3D &bbox) {
  NearAndFarEstimator fs(*this);
  fs.push(bbox.Xmin(), bbox.Ymin(), bbox.Zmin());
  fs.push(bbox.Xmax(), bbox.Ymin(), bbox.Zmin());
  fs.push(bbox.Xmin(), bbox.Ymax(), bbox.Zmin());
  fs.push(bbox.Xmax(), bbox.Ymax(), bbox.Zmin());
  fs.push(bbox.Xmin(), bbox.Ymin(), bbox.Zmax());
  fs.push(bbox.Xmax(), bbox.Ymin(), bbox.Zmax());
  fs.push(bbox.Xmin(), bbox.Ymax(), bbox.Zmax());
  fs.push(bbox.Xmax(), bbox.Ymax(), bbox.Zmax());

  near = fs.near;
  far = fs.far;
}

/////////////////////////////////////////////////
// OpenGL conversions

void Camera::OpenGLProjection(double *M) const {
  // from central pixel coordinates to corner pixel coordinates
  double T_[] = {1, 0, .5,
                 0, 1, .5,
                 0, 0,  1 };
  // The trasformation to normalized image coordinates [-1,1]x[-1,1].
  // Note that we don't reverse the y axis.  Therfore, if you use this to
  // render on screen graphics they will appear upside down
  double H_[] = {2./width,  0, -1,
                 0, 2./height, -1,
                 0,         0,  1 };
  vnl_double_3x3 T(T_), H(H_);

  // Kp is the K of the projection in normalized image coordinates
  vnl_double_3x3 Kp = H * T * K;

  // coeficients to compute the normalized z, see glFrustum doc
  const double C = (far + near) / (far - near);
  const double D = -2 * far * near / (far - near);

  M[0] = Kp(0,0);  M[4] = Kp(0,1);  M[ 8] = Kp(0,2);  M[12] = 0;
  M[1] = 0;        M[5] = Kp(1,1);  M[ 9] = Kp(1,2);  M[13] = 0;
  M[2] = 0;        M[6] = 0;        M[10] = C;        M[14] = D;
  M[3] = 0;        M[7] = 0;        M[11] = 1;        M[15] = 0;
}

void Camera::OpenGLModelview(double *M) const {
  M[0] = R(0,0);  M[4] = R(0,1);  M[ 8] = R(0,2);  M[12] = t(0);
  M[1] = R(1,0);  M[5] = R(1,1);  M[ 9] = R(1,2);  M[13] = t(1);
  M[2] = R(2,0);  M[6] = R(2,1);  M[10] = R(2,2);  M[14] = t(2);
  M[3] = 0;       M[7] = 0;       M[11] = 0;       M[15] = 1;
}

vnl_double_3x3 Camera::TexCoordsFromPixelCoordsHomography() {
  double w = width;
  double h = height;
  double H[] = {1/w,   0, .5/w,
                  0, 1/h, .5/h,
                  0,   0,  1    };
  return vnl_double_3x3(H);
}


// find the P = K(R|t) decomposition HZ 6.1 pag.157
void Camera::KRt_from_P(vnl_double_3x3 &K,
                        vnl_double_3x3 &R,
                        vnl_double_3 &t,
                        const vnl_double_3x4 &P ) {
  // decompose using the RQ decomposition HZ A4.1.1 pag.579
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      K(i,j) = P(i,j);
  vnl_double_3x3 Q = vnl_identity_3x3();

  // set K(2,1) to zero
  if (K(2,1) != 0) {
    double c = -K(2,2);
    double s = K(2,1);
    double l = sqrt(c * c + s * s);
    c /= l; s /= l;
    double qx[] = {1, 0,  0,
                   0, c, -s,
                   0, s,  c};
    vnl_double_3x3 Qx(qx);
    K = K * Qx;
    Q = Qx.transpose() * Q;
  }
  // set K(2,0) to zero
  if (K(2,0) != 0) {
    double c = K(2,2);
    double s = K(2,0);
    double l = sqrt(c * c + s * s);
    c /= l; s /= l;
    double qy[] = { c, 0, s,
                    0, 1, 0,
                   -s, 0, c };
    vnl_double_3x3 Qy(qy);
    K = K * Qy;
    Q = Qy.transpose() * Q;
  }
  // set K(1,0) to zero
  if (K(1,0) != 0) {
    double c = -K(1,1);
    double s = K(1,0);
    double l = sqrt(c * c + s * s);
    c /= l; s /= l;
    double qz[] = {c,-s, 0,
                   s, c, 0,
                   0, 0, 1};
    vnl_double_3x3 Qz(qz);
    K = K * Qz;
    Q = Qz.transpose() * Q;
  }

  R = Q;

  // ensure that the diagonal is positive
  if( K(2,2) < 0) {
    K = -K;
    R = -R;
  }
  if( K(1,1) < 0 ) {
    vnl_double_3x3 S = vnl_identity_3x3();
    S(1,1) = -1;
    K = K * S;
    R = S * R;
  }
  if( K(0,0) < 0 ) {
    vnl_double_3x3 S = vnl_identity_3x3();
    S(0,0) = -1;
    K = K * S;
    R = S * R;
  }

  // compute the translation vector
  t = vnl_inverse(K) * P.get_column(3);

  // scale K so that K(2,2) = 1
  K /= K(2,2);
}

vnl_double_3x3 Camera::tensor_product(const vnl_double_3 &a,
                                      const vnl_double_3 &b ) {
  vnl_double_3x3 res;
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      res(i,j) = a(i) * b(j);
  return res;
}

vnl_double_3x3 Camera::H_from_2P_plane(const Camera &c1,
                                       const Camera &c2,
                                       const vnl_double_4 &plane) {
  // rotate everything to put the camera at the origin
  vnl_double_3x3 R = c2.R * c1.R.transpose();
  vnl_double_3 t = - R * c1.t + c2.t;

  vnl_double_3 nn(plane(0), plane(1), plane(2));
  double dd = plane(3);

  // rotate the plane
  vnl_double_3 n = c1.R * nn;
  double d = -dot_product(n, c1.t) + dd;

  // homography equation from HZ 13.1 pag.327
  return c2.K * (R - tensor_product(t,n) / d) * vnl_inverse(c1.K);
}


vnl_double_4x3 Camera::ImageToPlaneHomography(const Camera &cam,
                                              const vnl_double_4 &plane) {
  // Combine the two equations
  //     P * x = u
  //     plane * x = 0
  // into
  //     A * x = (u, 0)^t
  double _A[16] = {
    cam.P(0,0), cam.P(0,1), cam.P(0,2), cam.P(0,3),
    cam.P(1,0), cam.P(1,1), cam.P(1,2), cam.P(1,3),
    cam.P(2,0), cam.P(2,1), cam.P(2,2), cam.P(2,3),
    plane(0), plane(1), plane(2), plane(3)
  };
  vnl_double_4x4 A(_A);

  // Invert A to get
  //     x = A^{-1} * (u, 0)^t
  vnl_double_4x4 A1 = vnl_inverse(A);

  // Discard last colomn of A^{-1}
  double _H[12] = {
    A1(0,0), A1(0,1), A1(0,2),
    A1(1,0), A1(1,1), A1(1,2),
    A1(2,0), A1(2,1), A1(2,2),
    A1(3,0), A1(3,1), A1(3,2)
  };

  return vnl_double_4x3(_H);                    
}

// homogenuous coordinates of a fronto-parallel plane with depth d
vnl_double_4 Camera::frontoparallel_plane_from_depth(double d) const {
  return vnl_double_4(R(2,0),R(2,1),R(2,2), t(2) - d);
}

int Camera::InSurface(int u, int v) const {
   return u >= 0 && v >= 0 && u < width && v < height;
}


double Camera::CameraDistance(const Camera &ci, const Camera &cj) {
  return (ci.ocenter - cj.ocenter).two_norm();
}

void Camera::KClosestCameras(const std::vector<Camera> &cameras,
                                    int key_view,
                                    int k,
                                    std::vector<int> *k_closest_cameras) {
  std::vector<std::pair<double, int> > distances;
  for (int i = 0; i < int(cameras.size()); ++i) {
    if (i != key_view) {
      double dist = CameraDistance(cameras[i], cameras[key_view]);
      distances.push_back(std::pair<double, int>(dist, i));
    }
  }
  std::sort(distances.begin(), distances.end());
  k_closest_cameras->resize(k);
  for(int i = 0; i < k; ++i) {
    (*k_closest_cameras)[i] = distances[i].second;
  }
}

