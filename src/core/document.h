#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "Eigen/Dense"

#include "geometry.h"
#include "camera.h"

using namespace Eigen;


inline Matrix3f PanTiltRoll(float pan, float tilt, float roll) {
  Matrix3f m;
  m = AngleAxisf(pan * M_PI / 180., Vector3f::UnitZ())
    * AngleAxisf(tilt * M_PI / 180., Vector3f::UnitY())
    * AngleAxisf(roll * M_PI / 180., Vector3f::UnitZ());
  return m;
}

inline AngleAxisf PanTiltRollA(float pan, float tilt, float roll) {
  return AngleAxisf(pan * M_PI / 180., Vector3f::UnitZ())
    * AngleAxisf(tilt * M_PI / 180., Vector3f::UnitY())
    * AngleAxisf(roll * M_PI / 180., Vector3f::UnitZ());
}


// TODO(pau): Add signals for document changed.
class SpDocument {

 public:
  SpDocument();
  virtual ~SpDocument();
  
  void LoadGeometry(const char *path);

  const Geometry &CaptureGeometry() const { return capture_geometry_; }
  const Geometry &SensorGeometry(int i) const { return sensor_geometry_[i]; }
  const Geometry &ScreenGeometry(int i) const { return screen_geometry_[i]; }
  const Geometry &TheaterGeometry() const { return theater_geometry_; }

  float FocalLegth() const { return focal_length_; }
  float SensorWidth() const { return sensor_width_; }
  float SensorHeight() const { return sensor_height_; }

  float RigInterocular() const { return rig_interocular_; }
  float RigConvergence() const { return rig_convergence_; }
  float RigX() const { return rig_position_[0]; }
  float RigY() const { return rig_position_[1]; }
  float RigZ() const { return rig_position_[2]; }
  float RigPan() const { return rig_pan_; }
  float RigTilt() const { return rig_tilt_; }
  float RigRoll() const { return rig_roll_; }
  Matrix3f RigRotation() const { return PanTiltRoll(rig_pan_, rig_tilt_, rig_roll_); }
  Vector3f CameraPosition(int i) const;

  float ScreenWidth() const { return screen_width_; }
  float ScreenHeight() const { return screen_height_; }

  float ObserverInterocular() const { return observer_interocular_; }
  float ObserverX() const { return observer_position_[0]; }
  float ObserverY() const { return observer_position_[1]; }
  float ObserverZ() const { return observer_position_[2]; }
  float ObserverPan() const { return observer_pan_; }
  float ObserverTilt() const { return observer_tilt_; }
  float ObserverRoll() const { return observer_roll_; }
  Matrix3f ObserverRotation() const { return PanTiltRoll(observer_pan_,
                                                   observer_tilt_,
                                                   observer_roll_); }
  Vector3f EyePosition(int i) const;
  
  float NearDistance() const { return near_distance_; }
  float FarDistance() const { return far_distance_; }
  float ParallaxFromDepth(float z) const;
  float NearParallax() const;
  float FarParallax() const;
  float ParallaxBudged() const;
  
 public:
  void SetFocalLegth(float v);
  void SetSensorWidth(float v);
  void SetSensorHeight(float v);

  void SetRigInterocular(float v);
  void SetRigConvergence(float v);
  void SetRigX(float v);
  void SetRigY(float v);
  void SetRigZ(float v);
  void SetRigPan(float v);
  void SetRigTilt(float v);
  void SetRigRoll(float v);
  
  void SetScreenWidth(float v);
  void SetScreenHeight(float v);
  
  void SetObserverInterocular(float v);
  void SetObserverX(float v);
  void SetObserverY(float v);
  void SetObserverZ(float v);
  void SetObserverPan(float v);
  void SetObserverTilt(float v);
  void SetObserverRoll(float v);
  
  void SetNearDistance(float v);
  void SetFarDistance(float v);

  void setDocumentChanged(bool b);
  bool DocumentChanged();

 private:
  void UpdateEverything();

  void ProjectToSensor();
  void SensorToScreen();

  void Triangulate();

 private:
  bool document_changed_;

  Geometry capture_geometry_;
  Geometry sensor_geometry_[2];   // left and right images
  Geometry screen_geometry_[2];   // left and right images
  Geometry theater_geometry_;

  // All distances are in mm.

  // Camera parameters.
  float focal_length_;
  float sensor_width_;
  float sensor_height_;

  // Rig parameters.
  float rig_interocular_;
  float rig_convergence_;
  Vector3f rig_position_;
  float rig_pan_, rig_tilt_, rig_roll_;
  
  // Screen parameters.
  // The center of the screen is assumed to be at (0,0,0), its top-left corner 
  // is at (screen_width_ / 2, screen_height_ / 2, 0).  The normal vector
  // pointing to the theater is the (0,0,1).
  
  float screen_width_;
  float screen_height_;
  
  // Observer parameters.
  float observer_interocular_;
  Vector3f observer_position_;
  float observer_pan_, observer_tilt_, observer_roll_;
  
  // Scene parameters.
  float near_distance_;
  float far_distance_;
};

#endif // DOCUMENT_H_
