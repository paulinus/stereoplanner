//
//  utils.m
//  StereoPlanner
//
//  Created by Pau Gargallo on 10/16/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "utils.h"


NSString *floatToString(float value, int significand_digits) {
  NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
  [numberFormatter setNumberStyle: NSNumberFormatterDecimalStyle];
  [numberFormatter setUsesSignificantDigits:true];
  [numberFormatter setMaximumSignificantDigits:significand_digits];
  [numberFormatter setRoundingMode: NSNumberFormatterRoundUp];
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

