#ifndef stereo_frustum_h_
#define stereo_frustum_h_


class StereoFrustum {
 public:
  StereoFrustum(float interocular, float convergence_distance,
                float width, float height, float dx, float dy)
  : b_(interocular), C_(convergence_distance), W_(width), H_(height),
    dx_(dx), dy_(dy) {
  }
  
  float ParallaxFromDepth(float z) const {
    return b_ / W_ * z / (z - C_);
  }
  
  float DetphFromParallax(float d) const {
    return b_ * C_ / (b_ - d * W_);
  }
  
  // TODO(pau): take into acount dx_ and dy_ here.
  void ToFrustumCoords(float x, float y, float z, float *u, float *v, float *d) {
    // TODO(pau): Finish this.
  }
  
  // TODO(pau): take into acount dx_ and dy_ here.
  void ViewAreaLeft(float Z, float *left, float *right, float *bottom,
                    float *top) const {
    *left = (Z * (b_ - W_) / C_ - b_) / 2;
    *right = (Z * (b_ + W_) / C_ - b_) / 2;
    *top = Z * H_ / C_ / 2;
    *bottom = - Z * H_ / C_ / 2;
  }
  
  // TODO(pau): take into acount dx_ and dy_ here.
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