#ifndef stereo_frustum_h_
#define stereo_frustum_h_


// TODO(pau): take into acount dx_ and dy_ here in all methods of the class.
class StereoFrustum {
 public:
  StereoFrustum(float interocular, float convergence_distance,
                float width, float height, float dx, float dy)
  : b_(interocular), C_(convergence_distance), W_(width), H_(height),
    dx_(dx), dy_(dy) {
  }
  
  float ParallaxFromDepth(float z) const {
    return b_ / W_ * (z - C_) / z;
  }
  
  float DepthFromParallax(float d) const {
    return b_ * C_ / (b_ - d * W_);
  }
  
  void WorldToFrustum(float x, float y, float z, float *u, float *v, float *d) {
    z = -z;
    *u = x / z * C_ / W_;
    *v = y / z * C_ / W_;
    *d = ParallaxFromDepth(z);
  }
  
  void FrustumToWorld(float u, float v, float d, float *x, float *y, float *z) {
    *z = DepthFromParallax(d);
    *x = *z * u * W_ / C_;
    *y = *z * v * W_ / C_;
    *z = -*z;
  }
  
  void ViewAreaLeft(float Z, float *left, float *right, float *bottom,
                    float *top) const {
    *left = (Z * (b_ - W_) / C_ - b_) / 2;
    *right = (Z * (b_ + W_) / C_ - b_) / 2;
    *top = Z * H_ / C_ / 2;
    *bottom = - Z * H_ / C_ / 2;
  }
  
  void ViewAreaRight(float Z, float *left, float *right, float *bottom,
                     float *top) const {
    *left = (Z * (-b_ - W_) / C_ + b_) / 2;
    *right = (Z * (-b_ + W_) / C_ + b_) / 2;
    *top = Z * H_ / C_ / 2;
    *bottom = - Z * H_ / C_ / 2;  
  }
  
 private:
  float b_; // The interoccular distance.
  float C_; // The convergence distance, or distance to the screen.
  float W_, H_; // The window width and height.
  float dx_, dy_; // The horitzontal and vertical shifts of the camera/observer
                  // with respect to the window.
};

#endif //stereo_frustum_h_