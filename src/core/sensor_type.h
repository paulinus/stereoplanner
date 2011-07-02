//
//  sensor_type.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 7/2/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <string>
#include <vector>


struct SensorType {
  SensorType(const char *_label, float _width, float _height) :
      label(_label), width(_width), height(_height) {
  }
  
  std::string label;
  float width;
  float height;
};

void CreateSensorTypeList(std::vector<SensorType> *list);
