#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <QObject>
#include <Eigen/Dense>

#include "geometry.h"
#include "camera.h"

using namespace Eigen;


// TODO(pau): Add signals for document changed.
class SpDocument : public QObject {
  Q_OBJECT

 public:
  SpDocument();
  virtual ~SpDocument();

  void UpdateEverything();

  Vector3d CameraPosition(int i);
  void ProjectToSensor();
  void SensorToScreen();
  Vector3d EyePosition(int i);

  void Triangulate();

  const Geometry &CaptureGeometry() const { return capture_geometry_; }
  const Geometry &SensorGeometry(int i) const { return sensor_geometry_[i]; }
  const Geometry &ScreenGeometry(int i) const { return screen_geometry_[i]; }
  const Geometry &TheaterGeometry() const { return theater_geometry_; }

  double FocalLegth() { return focal_length_; }
  double SensorWidth() { return sensor_width_; }
  double SensorHeight() { return sensor_height_; }

  double RigInterocular() { return rig_interocular_; }
  Vector3d RigPosition() { return rig_position_; }
  Quaterniond RigOrientation() { return rig_orientation_; }

  double ScreenWidth() { return screen_height_; }
  double ScreenHeight() { return screen_height_; }

  double ObserverInterocular() { return observer_interocular_; }
  Vector3d ObserverPosition() { return observer_position_; }
  Quaterniond ObserverOrientation() { return observer_orientation_; }

 private:
  Geometry capture_geometry_;
  Geometry sensor_geometry_[2];   // left and right images
  Geometry screen_geometry_[2];   // left and right images
  Geometry theater_geometry_;

  // All distances are in mm.

  // Camera parameters.
  double focal_length_;
  double sensor_width_;
  double sensor_height_;

  // Rig parameters.
  double rig_interocular_;
  Eigen::Vector3d rig_position_;
  Eigen::Quaterniond rig_orientation_;
  
  // Screen parameters.
  // The center of the screen is assumed to be at (0,0,0), its top-left corner 
  // is at (screen_width_ / 2, screen_height_ / 2, 0).  The normal vector
  // pointing to the theater is the (0,0,1).
  
  double screen_width_;
  double screen_height_;
  
  // Observer parameters.
  double observer_interocular_;
  Eigen::Vector3d observer_position_;
  Eigen::Quaterniond observer_orientation_;
};

#endif // DOCUMENT_H_
