//
//  utils.m
//  StereoPlanner
//
//  Created by Pau Gargallo on 10/16/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "utils.h"

float significantDigitsStep(float x, int significand_digits) {
  int k = floor(log10(x)) + 1;
  return pow(10, k - significand_digits);
}

float significantDigits(float x, int significand_digits) {
  float z = significantDigitsStep(x, significand_digits);
  return round(x / z) * z;
}

NSString *floatToString(float value, int significand_digits) {
  NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
  [numberFormatter setNumberStyle: NSNumberFormatterDecimalStyle];
  [numberFormatter setUsesSignificantDigits:true];
  [numberFormatter setMaximumSignificantDigits:significand_digits];
  [numberFormatter setRoundingMode: NSNumberFormatterRoundHalfDown];
  NSNumber *c = [NSNumber numberWithFloat:value];
  NSString *res = [numberFormatter stringFromNumber:c];
  [numberFormatter release];
  return res;
}

NSString *floatToStringInMetric(float meters, int significand_digits) {
  float x = meters * 1000;
  if (x < 1000) {
    return [NSString stringWithFormat:@"%@%@", 
            floatToString(x, significand_digits), @"mm"];
  }
  x /= 1000;
  if (x < 1000) {
    return [NSString stringWithFormat:@"%@%@", 
            floatToString(x, significand_digits), @"m"];   
  }
  x /= 1000;
  return [NSString stringWithFormat:@"%@%@", 
          floatToString(x, significand_digits), @"km"];   
}

NSString *floatToStringPercentage(float value, int significand_digits) {
  return [NSString stringWithFormat:@"%@%@", 
          floatToString(value, significand_digits), @"%"];
}

