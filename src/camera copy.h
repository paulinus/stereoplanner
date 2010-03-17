#ifndef __Camera_h__
#define __Camera_h__


#include <Eigen/Dense>

class Camera {
public:
  Matrix3f K;
  Matrix3f R;
  Vector3f t;

  Matrix34f P;
  Matrix3 KR1;  // KR^{-1} for computing unprojections faster
  Vector3f ocenter;

  int width, height;
  double near, far;


  /////////////////////////////////////////////////
  // Settings

  /** \brief Set the projection matrix.
   *
   *  \param P The projectiom matrix.
   *
   *  The function decomposes P in the form P = a K(R|t) with K(2,2) = 1.  Then
   *  P is recomputed as P = K(R|t).  All P, K, R and t of the camera are
   *  modified accordingly.
   */
  void SetP(const vnl_double_3x4 &P_);

  /** \brief Set internal and external camera parameters.
   *
   *  \param K The interal parameters matrix.  K is expected to be in the
   *           standard form.  See CheckK().
   *  \param R The rotation from world coordinates to camera coordinates.
   *  \param t The coordinates of the world origin in camera coordinatse.
   *
   * The projection matrix is computed as P = K(R|t).
   */
  void SetKRt(const vnl_double_3x3 &K_, const vnl_double_3x3 &R_,
               const vnl_double_3 &t_);
  
private:
  /** \brief Check and enforce that K has the required standard form.
   *
   *  \param tol A tolerance on the values of K.
   *  \return    True if the test passed.
   * 
   * First it is checked that, up to a given tolerance, the K matrix is
   * triangular superior and that K(2,2) = 1.  Then if the check passes, this
   * condition is made to hold exactly by modifying K.
   */
  bool CheckK(const double tol=1e-8);
  
  void UpdateKR1();
  
  void UpdateOpticalCenter();

public:
  /** \brief Return the 3x4 (R|t) matrix.
   */
  vnl_double_3x4 Rt();

  /** \brief Resize image plane and update K accordingly.
   *
   *  \param new_width The desired width of the image plane.
   *  \param new_height The desized height of the image plane.
   */
  void ImageResize(int new_width, int new_height);

  /** \brief Modifies near and far distances so that (x,y,z) is in between.
   * 
   *  \param x,y,z The coordinates of the point.
   */
  void IncludeBetweenNearAndFar(const REAL x, const REAL y, const REAL z);

  /** \brief Set near and far to the smaller and bigger depths of a bounding box.
   *
   *  \param bbox The bounding box.
   */
  void SetNearAndFarFromBoundingBox(BoundingBox3D &bbox);

  /** \brief Checks whether a 2D point is inside the image rectangle.
   *
   *  \param u,v The point.
   */
  int InSurface(int u, int v) const;

  
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
  template<typename T>
  void Project(T &u, T &v, T &zi, const T x, const T y, const T z) const {
    zi = P(2,0)*x + P(2,1)*y + P(2,2)*z + P(2,3);
    u = (P(0,0)*x + P(0,1)*y + P(0,2)*z + P(0,3)) / zi;
    v = (P(1,0)*x + P(1,1)*y + P(1,2)*z + P(1,3)) / zi;
  }

  /** \brief Computes the projection of a 3D point and its gradient.
   *
   *  \param [out] u  The x coordinate of the projected point.
   *  \param [out] z  The y coordinate of the projected point.
   *  \param [out] zi  The depth of the point with respect to the camera.
   *  \param [out] ux The derivative of the first coordinate of the projection
   *                  with respect to the x coordinate of the 3D point.
   *  \param [out] uy The derivative of the first coordinate of the projection
   *                  with respect to the y coordinate of the 3D point.
   *  \param [out] uz The derivative of the first coordinate of the projection
   *                  with respect to the z coordinate of the 3D point.
   *  \param [out] vx The derivative of the second coordinate of the projection
   *                  with respect to the x coordinate of the 3D point.
   *  \param [out] vy The derivative of the second coordinate of the projection
   *                  with respect to the y coordinate of the 3D point.
   *  \param [out] vz The derivative of the second coordinate of the projection
   *                  with respect to the z coordinate of the 3D point.
   *  \param [in]  x  The x coordinate of the 3D point.
   *  \param [in]  y  The y coordinate of the 3D point.
   *  \param [in]  z  The z coordinate of the 3D point.
   */
  template<typename T>
  void ProjectGradient(T &u,  T &v,  T &zi,
                       T &ux, T &uy, T &uz,
                       T &vx, T &vy, T &vz,
                       const T x, const T y, const T z ) const {
    const double xx = P(0,0)*x + P(0,1)*y + P(0,2)*z + P(0,3);
    const double yy = P(1,0)*x + P(1,1)*y + P(1,2)*z + P(1,3);
    zi = P(2,0)*x + P(2,1)*y + P(2,2)*z + P(2,3);
    u = xx / zi;
    v = yy / zi;
    const double zi2=zi*zi;
    ux = (P(0,0)*zi - P(2,0)*xx) / zi2;
    uy = (P(0,1)*zi - P(2,1)*xx) / zi2;
    uz = (P(0,2)*zi - P(2,2)*xx) / zi2;
    vx = (P(1,0)*zi - P(2,0)*yy) / zi2;
    vy = (P(1,1)*zi - P(2,1)*yy) / zi2;
    vz = (P(1,2)*zi - P(2,2)*yy) / zi2;
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
  void CameraCoordinates(double &xc, double &yc, double &zc,
                         double x, double y, double z ) const {
    xc = R(0,0)*x + R(0,1)*y + R(0,2)*z + t(0);
    yc = R(1,0)*x + R(1,1)*y + R(1,2)*z + t(1);
    zc = R(2,0)*x + R(2,1)*y + R(2,2)*z + t(2);
  }


  /////////////////////////////////////////////////
  // UnProjections

  /** \brief BackProjects an image point to a plane of constant depth.
   *
   *  \param u The x coordinate of the image point.
   *  \param v The y coordinate of the image point.
   *  \param z The depth at which we want to backproject.
   *
   * The backprojected point is defined by the following properties.
   * (1) The projection of the point into the image is (u,v)
   * (2) The depth of the point with respect to the camera is z.
   */
  vnl_double_3 BackProject(const double u,
                           const double v,
                           const double z) const {
    vnl_double_3 U(u*z, v*z, z);
    vnl_double_3 dir = KR1 * U;
    return ocenter + dir;
  }

  /** \brief BackProjects an image point to a plane.
   *
   *  \param u The x coordinate of the image point.
   *  \param v The y coordinate of the image point.
   *  \param plane The plane at which we want to backproject.
   *
   * The backprojected point is defined by the following properties.
   * (1) The projection of the point into the image is (u,v)
   * (2) The point belongs to the plane.
   *
   * Note that this function will fail if the plane contains the optical
   * center of the camera.
   */
  vnl_double_3 BackProject(const double x, const double y,
                           const vnl_double_4 &plane) const {
    vnl_double_3 v = KR1 * vnl_double_3(x, y, 1);
    double dotO =   plane(0) * ocenter(0)
                  + plane(1) * ocenter(1)
                  + plane(2) * ocenter(2)
                  + plane(3);
    double dotV =   plane(0) * v(0)
                  + plane(1) * v(1)
                  + plane(2) * v(2);

    double lambda = - dotO / dotV;

    return ocenter + lambda * v;
  }

  /** \brief Direction of the viewing ray of a pixel.
   *
   *  \param u The x coordinate of the image point.
   *  \param v The y coordinate of the image point.
   *  \return  a vector pointing to the direction of sigh of the image point.
   *           This is defined as a vector such that (1) the projection of
   *           the optical center plus the vector is (u,v), and (2) the depth
   *           of the optical center plus the vector is 1.
   *
   * \see IntrinsicPixelDirection
   */
  vnl_double_3 PixelDirection(const double u, const double v) const {
    vnl_double_3 direction(u, v, 1);
    return KR1 * direction;
  }
  
  /** \brief Direction of the viewing ray of a pixel in camera coordinates.
   *
   *  \param u The x coordinate of the image point.
   *  \param v The y coordinate of the image point.
   *  \return  a vector d pointing to the direction of sigh of the image point.
   *           This is defined K^{-1} d.
   * 
   * \see PixelDirection, CameraCoordinates.
   */
  vnl_double_3 IntrinsicPixelDirection(const double u, const double v) const {
    vnl_double_3 direction(u, v, 1);
    return vnl_inverse(K) * direction;
  }

  /////////////////////////////////////////////////
  // OpenGL conversions

  /** \brief Convert intrinsic parameters into OpenGL format.
   *
   *  \param [out] M  The OpenGL projection matrix corresponding to this camera.
   *                  This is a pointer to a column major 4x4 matrix as required
   *                  by glLoadMatrixd().
   *
   * Note that besides K, this function uses also the near and far depth
   * to compute the transformation of the z coordinate as in glFrustum().
   *
   * \see OpenGLModelview
   */
  void OpenGLProjection(double *M) const;


  /** \brief Convert extrinsic parameters into OpenGL format.
   *
   *  \param [out] M  The OpenGL model-view matrix corresponding to this camera.
   *                  This is a pointer to a column major 4x4 matrix as required
   *                  by glLoadMatrixd().
   * \see OpenGLProjection
   */
  void OpenGLModelview(double *M) const;

  /** \brief Compute the depth of a pixel given its OpenGL depth-buffer value.
   *
   * \param z The value of the OpenGL depth buffer.
   * \return  The actual depth corresponding to the depth buffer value.
   *
   * See glFrustum() documentation to see the relation between depth buffer
   * values, which are coordinates in the Frustum, and the actual depth.
   */
  double DepthFromOpenGLDepthBuffer(double z) const {
    const double C = (far + near) / (far - near);
    const double D = - 2 * far * near / (far - near);

    return D / (2 * z - 1 - C);
  }

  /** \brief Homography transforming pixel coords to OpenGL texture coords.
   *
   * In vision, image coordinates range from (0,0) to (w - 1, height - 1).
   * In OpenGl textures, they range from (0,0) to (1,1).  This functions
   * computes the transforamtion form the vision coordinate system to the
   * OpenGL texture one.
   */
  vnl_double_3x3 TexCoordsFromPixelCoordsHomography();

  ////////////////////////////////////////////////
  // Static functions

  /** \brief Distance between the optical center of two cameras.
   *
   *  \param ci A camera.
   *  \param cj Another camera.
   */
  static double CameraDistance(const Camera &ci, const Camera &cj);

  /** \brief Finds the k cameras that are closer to a given one.
   *
   *  \param cameras A vector containing al the cameras.
   *  \param key_view The index of the camera to which the closest cameras are
   *                  to be computed.
   *  \param k The number of closest cameras to search.
   *  \param [out] k_closest_cameras A vector with the indices of the k closest
   *               cameras
   */
  static void KClosestCameras(const std::vector<Camera> &cameras,
                              int key_view,
                              int k,
                              std::vector<int> *k_closest_cameras);
public:

  // find the P = K(R|t) decomposition HZ 6.1 pag.157
  static void KRt_from_P(vnl_double_3x3 &K, vnl_double_3x3 &R, vnl_double_3 &t,
                         const vnl_double_3x4 &P );
  
  static vnl_double_3x3 tensor_product(const vnl_double_3 &a,
                                       const vnl_double_3 &b );
  
  static vnl_double_3x3 H_from_2P_plane(const Camera &c1, const Camera &c2,
                                        const vnl_double_4 &plane);
  
  /** Computes the homography that transforms pixels in the image plane to
   *  their backprojection into a 3d plane.
   *
   *  \param cam   The camera.
   *  \param plane The coordinates of the plane equation.
   *  \return      The transformation matrix from the image to the plane.
   *               This is a 4x3 matrix since in maps from 2D to 3D.
   */
  static vnl_double_4x3 ImageToPlaneHomography(const Camera &cam,
                                               const vnl_double_4 &plane);
  
  // homogenuous coordinates of a fronto-parallel plane with depth d
  vnl_double_4 frontoparallel_plane_from_depth(double d) const;
};


class NearAndFarEstimator {
public:
  const Camera &c;
  double near,far;

  NearAndFarEstimator(const Camera &cam) : c(cam) {
    near = 1e20;
    far = 0;
  }

  void push(const REAL x, const REAL y, const REAL z) {
    REAL u, v, zi;
    c.Project(u, v, zi, x, y, z);
    if(zi < near) near = zi;
    if(far < zi) far = zi;
  }
};

#endif //__Camera_h__
