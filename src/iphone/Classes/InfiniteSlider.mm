//
//  EAGLView.m
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "utils.h"
#import "InfiniteSlider.h"


@implementation InfiniteSlider

//@synthesize label;
@dynamic value;
@synthesize valueType;


- (id) init {
  self = [super init];
  value = 1;
  valueType = ISmetric;
  
  label_view = [[UILabel alloc] init];
  [self addSubview:label_view];
  [label_view setTextAlignment:UITextAlignmentCenter];

  value_view = [[UILabel alloc] init];
  [self addSubview:value_view];
  [value_view setTextAlignment:UITextAlignmentCenter];
  
  return self;
}

- (void) setFrame:(CGRect)rect {
  [super setFrame:rect];
  rect.origin.x = 0;
  rect.origin.y = 0;
  rect.size.height /= 2;
  [label_view setFrame:rect];
  rect.origin.y += rect.size.height;
  [value_view setFrame:rect];

}

- (void) setLabel:(NSString *)l {
  [label_view setText:l];
}

- (void) setValue:(float)v {
  value = v;
  
  NSString *valueString;
  switch (valueType) {
    case ISmetric:
      valueString = floatToStringInMetric(value, 3);
      break;
    case ISpercentage:
      valueString = floatToStringPercentage(100 * value, 3);
      break;
    default:
      valueString = floatToString(value, 3);
      break;
  }
  [value_view setText:valueString];
  
  [self setNeedsDisplay];
  [self sendActionsForControlEvents:UIControlEventValueChanged];
}

- (float) value {
  return value;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray *allTouches = [[event allTouches] allObjects];
  
  if ([allTouches count] == 1) {
    UITouch *touch = [touches anyObject];
    
    CGPoint p1 = [touch previousLocationInView:self];
    CGPoint p2 = [touch locationInView:self];
    
    self.value = self.value * pow(2.0, (float)(p2.x - p1.x) / 100.);
  }
}

@end
