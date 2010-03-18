#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <QObject>
#include <Eigen/Dense>

#include "geometry.h"
#include "camera.h"

using namespace Eigen;


inline Matrix3d PanTiltRoll(double pan, double tilt, double roll) {
  Matrix3d m;
  m = AngleAxisd(pan, Vector3d::UnitZ())
    * AngleAxisd(tilt, Vector3d::UnitY())
    * AngleAxisd(roll, Vector3d::UnitZ());
  return m;
}


// TODO(pau): Add signals for document changed.
class SpDocument : public QObject {
  Q_OBJECT

 public:
  SpDocument();
  virtual ~SpDocument();

  const Geometry &CaptureGeometry() const { return capture_geometry_; }
  const Geometry &SensorGeometry(int i) const { return sensor_geometry_[i]; }
  const Geometry &ScreenGeometry(int i) const { return screen_geometry_[i]; }
  const Geometry &TheaterGeometry() const { return theater_geometry_; }

  double FocalLegth() { return focal_length_; }
  double SensorWidth() { return sensor_width_; }
  double SensorHeight() { return sensor_height_; }

  double RigInterocular() { return rig_interocular_; }
  Vector3d RigPosition() { return rig_position_; }
  double RigPan() { return rig_pan_; }
  double RigTilt() { return rig_tilt_; }
  double RigRoll() { return rig_roll_; }
  Matrix3d RigRotation() { return PanTiltRoll(rig_pan_, rig_tilt_, rig_roll_); }
  Vector3d CameraPosition(int i);

  double ScreenWidth() { return screen_height_; }
  double ScreenHeight() { return screen_height_; }

  double ObserverInterocular() { return observer_interocular_; }
  Vector3d ObserverPosition() { return observer_position_; }
  double ObserverPan() { return observer_pan_; }
  double ObserverTilt() { return observer_tilt_; }
  double ObserverRoll() { return observer_roll_; }
  Matrix3d ObserverRotation() { return PanTiltRoll(observer_pan_,
                                                   observer_tilt_,
                                                   observer_roll_); }
  Vector3d EyePosition(int i);
 
 public slots:
  void SetFocalLegth(double v);
  void SetSensorWidth(double v);
  void SetSensorHeight(double v);

  void SetRigInterocular(double v);
  void SetRigPosition(double x, double y, double z);
  void SetRigOrientation(double p, double t, double r);
  
  void SetScreenWidth(double v);
  void SetScreenHeight(double v);
  
  void SetObserverInterocular(double v);
  void SetObserverPosition(double x, double y, double z);
  void SetObserverOrientation(double p, double t, double r);

 signals:
  void DocumentChanged();

 private:
  void UpdateEverything();

  void ProjectToSensor();
  void SensorToScreen();

  void Triangulate();

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
  Vector3d rig_position_;
  double rig_pan_, rig_tilt_, rig_roll_;
  
  // Screen parameters.
  // The center of the screen is assumed to be at (0,0,0), its top-left corner 
  // is at (screen_width_ / 2, screen_height_ / 2, 0).  The normal vector
  // pointing to the theater is the (0,0,1).
  
  double screen_width_;
  double screen_height_;
  
  // Observer parameters.
  double observer_interocular_;
  Vector3d observer_position_;
  double observer_pan_, observer_tilt_, observer_roll_;
};

#endif // DOCUMENT_H_
