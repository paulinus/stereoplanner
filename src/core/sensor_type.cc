//
//  sensor_type.cc
//  StereoPlanner
//
//  Created by Pau Gargallo on 7/2/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "sensor_type.h"


void CreateSensorTypeList(std::vector<SensorType> *list) {
  list->push_back(SensorType("NikonFX", 0.036, 0.024));
  list->push_back(SensorType("NikonDX", 0.0236, 0.0156));
}