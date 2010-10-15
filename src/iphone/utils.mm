//
//  utils.m
//  StereoPlanner
//
//  Created by Pau Gargallo on 10/16/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "utils.h"


NSString *floatToString(float x, int num_decimals) {
  NSString *res = [NSString stringWithFormat:@"%0.2f", x];
  int dot = 0;
  for (int i = 0; i < [res length]; ++i) {
    if([res characterAtIndex:i] == '.') {
      dot = i;
      break;
    }					  
  }
  
  if (dot) {
    int lastZero;
    for (lastZero = [res length] - 1; lastZero >= dot; --lastZero) {
      if([res characterAtIndex:lastZero] != '0') {
        break;
      }
    }	
    if (lastZero == dot)
      return [res substringToIndex:lastZero];
    else 
      return [res substringToIndex:lastZero + 1];
    
  } else {
    return res;
  }
}
