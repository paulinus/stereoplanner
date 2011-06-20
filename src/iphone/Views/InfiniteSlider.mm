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


- (void)commonInit {
  value = 1;
  valueType = ISmetric;
  
  minus_button = [[UIButton alloc] init];
  [self addSubview:minus_button];
  [minus_button setTitle:@"−" forState:UIControlStateNormal];
  [minus_button addTarget:self action:@selector(minusButtonAction) forControlEvents:UIControlEventTouchUpInside];

  label_view = [[UILabel alloc] init];
  [self addSubview:label_view];
  [label_view setTextAlignment:UITextAlignmentLeft];
  [label_view setTextColor:[UIColor whiteColor]];
  [label_view setBackgroundColor:[UIColor clearColor]];
  [label_view setFont:[UIFont fontWithName:@"Helvetica" size:18]];
  
  value_view = [[UILabel alloc] init];
  [self addSubview:value_view];
  [value_view setTextAlignment:UITextAlignmentRight];
  [value_view setTextColor:[UIColor yellowColor]];
  [value_view setBackgroundColor:[UIColor clearColor]];
  [value_view setFont:[UIFont fontWithName:@"Helvetica" size:24]];

  plus_button = [[UIButton alloc] init];
  [self addSubview:plus_button];
  [plus_button setTitle:@"+" forState:UIControlStateNormal];
  [plus_button addTarget:self action:@selector(plusButtonAction) forControlEvents:UIControlEventTouchUpInside];
}

- (id)init {
  self = [super init];
  [self commonInit];
  return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
  self = [super initWithCoder:aDecoder];
  [self commonInit];
  return self;
}

- (void)setFrame:(CGRect)rect {
  [super setFrame:rect];
  float total_width = rect.size.width;
  float margin = 0;
  float button_width = 30;
  
  rect.origin.x = margin;
  rect.origin.y = 0;
  rect.size.width = button_width;
  [minus_button setFrame:rect];

  rect.origin.x += rect.size.width;
  rect.size.width = total_width / 2 - margin - button_width;
  [label_view setFrame:rect];
  
  rect.origin.x += rect.size.width;
  [value_view setFrame:rect];
  
  rect.origin.x += rect.size.width;
  rect.size.width = button_width;
  [plus_button setFrame:rect];
}

- (void)awakeFromNib {
  [self setFrame:[self frame]];
}

- (void)setLabel:(NSString *)l {
  [label_view setText:l];
}

- (void)setValue:(float)v {
  value = v;
  
  NSString *valueString;
  switch (valueType) {
    case ISmetric:
      valueString = floatToStringInMetric(value, 2);
      break;
    case ISpercentage:
      valueString = floatToStringPercentage(100 * value, 2);
      break;
    default:
      valueString = floatToString(value, 2);
      break;
  }
  [value_view setText:valueString];
  
  [self setNeedsDisplay];
  [self sendActionsForControlEvents:UIControlEventValueChanged];
}

- (float)value {
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

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  self.value = self.value = significantDigits(self.value, 2);
}

- (void)minusButtonAction {
  float new_v = self.value - significantDigitsStep(self.value, 2);
  self.value = significantDigits(new_v, 2);
}

- (void)plusButtonAction {
  float new_v = self.value + significantDigitsStep(self.value, 2);
  self.value = significantDigits(new_v, 2);
}

@end
