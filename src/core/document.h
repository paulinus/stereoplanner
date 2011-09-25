#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "Eigen/Dense"

#include "sensor_type.h"
#include "object.h"
#include "geometry.h"
#include "camera.h"
#include "stereo_frustum.h"

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
  
  void AddObject(const char *path);
  void RemoveObject(int i);

  const Geometry &CaptureGeometry();
  const Geometry &FrustumGeometry();
  const Geometry &TheaterGeometry();

  float FocalLegth() const { return focal_length_; }
  float SensorWidth() const { return sensor_width_; }
  float SensorHeight() const { return sensor_height_; }

  float RigInterocular() const { return rig_interocular_; }
  float RigConvergence() const { return rig_convergence_; }
  float StereoWindowWidth() const;
  float StereoWindowHeight() const;
  float RigX() const { return rig_position_[0]; }
  float RigY() const { return rig_position_[1]; }
  float RigZ() const { return rig_position_[2]; }
  float RigPan() const { return rig_pan_; }
  float RigTilt() const { return rig_tilt_; }
  float RigRoll() const { return rig_roll_; }
  Matrix3f RigRotation() const { return PanTiltRoll(rig_pan_, rig_tilt_, rig_roll_); }
  Vector3f RigPosition() const { return rig_position_; }
  Vector3f CameraPosition(int i) const;

  float ScreenWidth() const { return screen_width_; }
  float ScreenHeight() const { return ScreenWidth() / SensorWidth() * SensorHeight(); }

  float ObserverInterocular() const { return observer_interocular_; }
  float ObserverX() const { return observer_position_[0] * ScreenWidth(); }
  float ObserverY() const { return observer_position_[1] * ScreenWidth(); }
  float ObserverZ() const { return observer_position_[2] * ScreenWidth(); }
  float ObserverXRatio() const { return observer_position_[0]; }
  float ObserverYRatio() const { return observer_position_[1]; }
  float ObserverZRatio() const { return observer_position_[2]; }
  float ObserverPan() const { return observer_pan_; }
  float ObserverTilt() const { return observer_tilt_; }
  float ObserverRoll() const { return observer_roll_; }
  Matrix3f ObserverRotation() const { return PanTiltRoll(observer_pan_,
                                                   observer_tilt_,
                                                   observer_roll_); }
  Vector3f EyePosition(int i) const;
  
  StereoFrustum ShootingFrustrum() const;
  StereoFrustum ViewingFrustrum() const;
  
  float NearDistance() const { return near_distance_; }
  float FarDistance() const { return far_distance_; }
  float MinParallaxConstraint() const { return min_parallax_constraint_; }
  float MaxParallaxConstraint() const { return max_parallax_constraint_; }
  float MaxBracketConstraint() const { return max_bracket_constraint_; }
  float MinScreenParallaxConstraint() const { return min_screen_parallax_constraint_; }
  float MaxScreenParallaxConstraint() const { return max_screen_parallax_constraint_; }
  float MaxScreenBracketConstraint() const { return max_screen_bracket_constraint_; }


  float NearParallax() const;
  float FarParallax() const;
  float ParallaxBudged() const;
  float NearScreenParallax() const;
  float FarScreenParallax() const;
  float ScreenParallaxBudged() const;
  float Roundness() const;
  float MaxRigInterocular() const;
  float MaxRigInterocularMinParallax() const;
  float MaxRigInterocularMaxParallax() const;
  float MaxRigInterocularMaxBracket() const;
  float MaxRigInterocularMinScreenParallax() const;
  float MaxRigInterocularMaxScreenParallax() const;
  float MaxRigInterocularMaxScreenBracket() const;
 
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
  
  void SetObserverInterocular(float v);
  void SetObserverXRatio(float v);
  void SetObserverYRatio(float v);
  void SetObserverZRatio(float v);
  void SetObserverPan(float v);
  void SetObserverTilt(float v);
  void SetObserverRoll(float v);
  
  void SetNearDistance(float v);
  void SetFarDistance(float v);
  
  void SetMinParallaxConstraint(float v);
  void SetMaxParallaxConstraint(float v);
  void SetMaxBracketConstraint(float v);
  void SetMinScreenParallaxConstraint(float v);
  void SetMaxScreenParallaxConstraint(float v);
  void SetMaxScreenBracketConstraint(float v);
  
  void SetSelectedSensorType(int i);
  int SelectedSensorType() const { return selected_sensor_type_; }
  const SensorType &SensorTypeAt(int i) { return sensor_type_list_[i]; }
  int NumSensorTypes() { return sensor_type_list_.size(); }

  void setDocumentChanged(bool b);
  bool DocumentChanged();

  void UpdateEverything(); //TODO(pau): make this private

 private:
  void UpdateCaptureGeometry();
  void UpdateFrustumGeometry();
  void UpdateTheaterGeometry();

 private:
  bool document_changed_;
 public:
  Object scene_; //TODO(pau): make this private
 private:
  Geometry capture_geometry_;
  bool capture_geometry_updated_;
  Geometry frustum_geometry_;
  bool frustum_geometry_updated_;
  Geometry theater_geometry_;
  bool theater_geometry_updated_;

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
  // is at (ScreenWidth() / 2, ScreenHeight() / 2, 0).  The normal vector
  // pointing to the theater is the (0,0,1).
  
  float screen_width_;
  
  // Observer parameters.
  float observer_interocular_;
  Vector3f observer_position_;  // As a portion of the screen width.
  float observer_pan_, observer_tilt_, observer_roll_;
  
  // Scene parameters.
  float near_distance_;
  float far_distance_;
  
  // Constraint parametres.
  float min_parallax_constraint_;
  float max_parallax_constraint_;
  float max_bracket_constraint_;
  float min_screen_parallax_constraint_;
  float max_screen_parallax_constraint_;
  float max_screen_bracket_constraint_;
  
  // Sensor Type list and selection.
  std::vector<SensorType> sensor_type_list_;
  int selected_sensor_type_;
};

#endif // DOCUMENT_H_
