#ifndef stereo_frustum_h_
#define stereo_frustum_h_


class StereoFrustum {
 public:
  StereoFrustum(float interocular, float convergence_distance,
                float width, float height, float dx, float dy)
  : b_(interocular), C_(convergence_distance), W_(width), H_(height),
    dx_(dx), dy_(dy) {
  }
  
  
 private:
  float b_; // The interoccular distance.
  float C_; // The convergence distance, or distance to the screen.
  float W_, H_; // The window width and height.
  float dx_, dy_; // The horitzontal and vertical shifts of the camera/observer
                  // with respect to the window.
};

#endif //stereo_frustum_h_