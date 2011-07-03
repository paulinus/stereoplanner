//
//  sensor_type.cc
//  StereoPlanner
//
//  Created by Pau Gargallo on 7/2/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "sensor_type.h"


void CreateSensorTypeList(std::vector<SensorType> *list) {
  list->push_back(SensorType("1/8\"",                                        1.60  / 1000., 1.20 / 1000.));
  list->push_back(SensorType("1/6\"",                                        2.40  / 1000., 1.80 / 1000.));
  list->push_back(SensorType("1/4\"",                                        3.20  / 1000., 2.40 / 1000.));
  list->push_back(SensorType("1/3.6\"",                                      4.00  / 1000., 3.00 / 1000.));
  list->push_back(SensorType("1/3.2\"",                                      4.54  / 1000., 3.42 / 1000.));
  list->push_back(SensorType("1/3\"",                                        4.80  / 1000., 3.60 / 1000.));
  list->push_back(SensorType("1/2.7\"",                                      5.37  / 1000., 4.04 / 1000.));
  list->push_back(SensorType("1/2.5\"",                                      5.76  / 1000., 4.29 / 1000.));
  list->push_back(SensorType("1/2.3\"",                                      6.16  / 1000., 4.62 / 1000.));
  list->push_back(SensorType("1/2\"",                                        6.40  / 1000., 4.80 / 1000.));
  list->push_back(SensorType("1/1.8\"",                                      7.18  / 1000., 5.32 / 1000.));
  list->push_back(SensorType("1/1.7\"",                                      7.60  / 1000., 5.70 / 1000.));
  list->push_back(SensorType("1/1.6\"",                                      8.08  / 1000., 6.01 / 1000.));
  list->push_back(SensorType("2/3\"",                                        8.80  / 1000., 6.60 / 1000.));
  list->push_back(SensorType("Super 16mm",                                   12.52 / 1000., 7.41 / 1000.));
  list->push_back(SensorType("1\"",                                          12.8  / 1000., 9.6  / 1000.));
  list->push_back(SensorType("4/3\" (Four Thirds)",                          17.3  / 1000., 13.0 / 1000.));
  list->push_back(SensorType("Sigma Foveon X3",                              20.7  / 1000., 13.8 / 1000.));
  list->push_back(SensorType("Canon APS-C",                                  22.2  / 1000., 14.8 / 1000.));
  list->push_back(SensorType("Sony, Pentax, Sigma, Samsung APS-C, Nikon DX", 23.6  / 1000., 15.6 / 1000.));
  list->push_back(SensorType("Canon APS-H",                                  27.9  / 1000., 18.6 / 1000.));
  list->push_back(SensorType("35mm Full-frame, Nikon FX",                    36    / 1000., 24   / 1000.));
  list->push_back(SensorType("Leica S2",                                     45    / 1000., 30   / 1000.));
  list->push_back(SensorType("Pentax 645D",                                  44    / 1000., 33   / 1000.));
  list->push_back(SensorType("Kodak KAF 39000 CCD",                          49    / 1000., 36.8 / 1000.));
  list->push_back(SensorType("Leaf AFi 10",                                  56    / 1000., 36   / 1000.));
  list->push_back(SensorType("Phase One P 65+",                              53.9  / 1000., 40.4 / 1000.));
}
