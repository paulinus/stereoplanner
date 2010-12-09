#ifndef OBJECT_H_
#define OBJECT_H_

#include <vector>
#include "Eigen/Dense"
#include "geometry.h"

class Object {
 public:
  Object() {
    position_ << 0, 0, 0;
    orientation_ = Eigen::Quaternionf(1, 0, 0, 0);
  }

 public:
  std::string name_;
  Geometry geometry_;  
  Eigen::Vector3f position_;
  Eigen::Quaternionf orientation_;
  
  std::vector<Object *> children_;
};


#endif //OBJECT_H_

