#include "document.h"


SpDocument::SpDocument() {
  focal_length_ = 0.040;
  sensor_width_ = 0.036;
  sensor_height_ = 0.024;

  rig_interocular_ = 0.065;
  rig_convergence_ = 7;
  rig_position_ << -7, 1, -4;
  rig_pan_ = 0;
  rig_tilt_ = 230;
  rig_roll_ = 0;

  screen_width_ = 10;

  observer_interocular_ = 0.065;
  observer_position_ << 0, 0, 10;
  observer_pan_ = 0;
  observer_tilt_ = 0;
  observer_roll_ = 0;
  
  near_distance_ = 5;
  far_distance_ = 10;

  capture_geometry_ = CubeGeometry();
  UpdateEverything();
}

SpDocument::~SpDocument() {
}

void SpDocument::LoadGeometry(const char *path) {
  ReadGeo(path, &capture_geometry_);
  UpdateEverything();
}

// TODO(pau): Consired non off-axis configuration (see doc/geometry.tex);
float SpDocument::StereoWindowWidth() const {
  return SensorWidth() * RigConvergence() / FocalLegth();
}

float SpDocument::StereoWindowHeight() const {
  return SensorHeight() * RigConvergence() / FocalLegth();
}

StereoFrustum SpDocument::ShootingFrustrum() const {
  return StereoFrustum(RigInterocular(), RigConvergence(), StereoWindowWidth(),
                       StereoWindowHeight(), 0, 0);
}

StereoFrustum SpDocument::ViewingFrustrum() const {
  return StereoFrustum(ObserverInterocular(), ObserverZ(), ScreenWidth(),
                       ScreenHeight(), ObserverX(), ObserverY());  
}


float SpDocument::ParallaxFromDepth(float z) const {
  return RigInterocular() / StereoWindowWidth()
         * (z - RigConvergence()) / z;
}

float SpDocument::ViewDepthFromShootingDepth(float z) const {
  float d = ParallaxFromDepth(z);
  return ObserverInterocular() * fabs(ObserverZ())
        / (ObserverInterocular() - d * ScreenWidth());
}

float SpDocument::NearParallax() const {
  return ParallaxFromDepth(NearDistance());
}

float SpDocument::FarParallax() const {
  return ParallaxFromDepth(FarDistance());
}

float SpDocument::ParallaxBudged() const {
  return FarParallax() - NearParallax();
}

float SpDocument::NearScreenParallax() const {
  return ScreenWidth() * NearParallax();
}

float SpDocument::FarScreenParallax() const {
  return ScreenWidth() * FarParallax();
}

float SpDocument::ScreenParallaxBudged() const {
  return FarScreenParallax() - NearScreenParallax();
}

void SpDocument::SetFocalLegth(float v) {
  if (focal_length_ != v) {
    focal_length_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetSensorWidth(float v) {
  if (sensor_width_ != v) {
    sensor_width_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetSensorHeight(float v) {
  if (sensor_height_ != v) {
    sensor_height_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigInterocular(float v) {
  if (rig_interocular_ != v) {
    rig_interocular_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigConvergence(float v) {
  if (rig_convergence_ != v) {
    rig_convergence_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigX(float v) {
  if (rig_position_[0] != v) {
    rig_position_[0] = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigY(float v) {
  if (rig_position_[1] != v) {
    rig_position_[1] = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigZ(float v) {
  if (rig_position_[2] != v) {
    rig_position_[2] = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigPan(float v) {
  if (rig_pan_ != v) {
    rig_pan_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigTilt(float v) {
  if (rig_tilt_ != v) {
    rig_tilt_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetRigRoll(float v) {
  if (rig_roll_ != v) {
    rig_roll_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetScreenWidth(float v) {
  if (screen_width_ != v) {
    screen_width_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetObserverInterocular(float v) {
  if (observer_interocular_ != v) {
    observer_interocular_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetObserverX(float v) {
  if (observer_position_[0] != v) {
    observer_position_[0] = v;
    UpdateEverything();
  }
}

void SpDocument::SetObserverY(float v) {
  if (observer_position_[1] != v) {
    observer_position_[1] = v;
    UpdateEverything();
  }
}

void SpDocument::SetObserverZ(float v) {
  if (observer_position_[2] != v) {
    observer_position_[2] = v;
    UpdateEverything();
  }
}

void SpDocument::SetObserverPan(float v) {
  if (observer_pan_ != v) {
    observer_pan_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetObserverTilt(float v) {
  if (observer_tilt_ != v) {
    observer_tilt_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetObserverRoll(float v) {
  if (observer_roll_ != v) {
    observer_roll_ = v;
    UpdateEverything();
  }
}


void SpDocument::SetNearDistance(float v) {
  if (near_distance_ != v) {
    near_distance_ = v;
    UpdateEverything();
  }
}

void SpDocument::SetFarDistance(float v) {
  if (far_distance_ != v) {
    far_distance_ = v;
    UpdateEverything();
  }
}


void SpDocument::setDocumentChanged(bool b) {
  document_changed_ = b;
}

bool SpDocument::DocumentChanged() {
  return document_changed_;
}

void SpDocument::UpdateEverything() {
  UpdateFrustumGeometry();
  UpdateTheaterGeometry();
  setDocumentChanged(true);
}

Vector3f SpDocument::CameraPosition(int i) const {
  Vector3f shift((i==0?-1:1) * rig_interocular_ / 2, 0, 0);
  return rig_position_ + RigRotation() * shift;
}

void SpDocument::UpdateFrustumGeometry() {
  Matrix3f R = RigRotation().transpose();
  Vector3f t = - R * RigPosition();
  Matrix4f T;
  T << R, t,
       MatrixXf::Zero(1,3), 1;
  
  StereoFrustum f = ShootingFrustrum();
  frustum_geometry_ = capture_geometry_;
  for (unsigned int i = 0; i < frustum_geometry_.vertex_.size(); i += 4) {
    Vector4f pp(&capture_geometry_.vertex_[i]);
    Vector4f p = T * pp;
    float *q = &frustum_geometry_.vertex_[i];

    f.WorldToFrustum(p[0] / p[3], p[1] / p[3], p[2] / p[3], q, q + 1, q + 2);
    q[3] = 1;
  }
}

Vector3f SpDocument::EyePosition(int i) const {
  Vector3f shift((i==0?-1:1) * observer_interocular_ / 2, 0, 0);
  return observer_position_ 
    + ObserverRotation() * shift;
}


void SpDocument::UpdateTheaterGeometry() {
  StereoFrustum f = ViewingFrustrum();
  theater_geometry_ = frustum_geometry_;
  for (unsigned int i = 0; i < theater_geometry_.vertex_.size(); i += 4) {
    float *p = &frustum_geometry_.vertex_[i];
    float *q = &theater_geometry_.vertex_[i];
    
    f.FrustumToWorld(p[0] / p[3], p[1] / p[3], p[2] / p[3], q, q + 1, q + 2);
    q[3] = 1;
  }
  theater_geometry_.ComputeNormals();
}

