//
//  utils.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 10/16/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

float significantDigitsStep(float x, int significand_digits);
float significantDigits(float x, int significand_digits);
NSString *floatToString(float value, int significant_digits);
NSString *floatToStringInMetric(float millimeters, int significant_digits);
NSString *floatToStringPercentage(float value, int significant_digits);
