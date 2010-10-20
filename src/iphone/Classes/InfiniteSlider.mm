//
//  EAGLView.m
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "utils.h"
#import "InfiniteSlider.h"

CGColorRef CreateDeviceGrayColor(CGFloat w, CGFloat a) {
  CGColorSpaceRef gray = CGColorSpaceCreateDeviceGray();
  CGFloat comps[] = {w, a};
  CGColorRef color = CGColorCreate(gray, comps);
  CGColorSpaceRelease(gray);
  return color;
}	

CGColorRef graphBackgroundColor() {
  static CGColorRef c = NULL;
  if(c == NULL)
  {
    c = CreateDeviceGrayColor(0.6, 1.0);
  }
  return c;
}


@implementation InfiniteSlider

@dynamic value;
@synthesize label;

- (id) init {
  value = 1;
  return [super init];
}

- (void) setValue:(float)v {
  value = v;
  [self setNeedsDisplay];
  [self sendActionsForControlEvents:UIControlEventValueChanged];
}

- (float) value {
  return value;
}

- (void) drawRect:(CGRect)rect {
  CGContextRef context = UIGraphicsGetCurrentContext();
  
  // Fill in the background
  CGContextSetFillColorWithColor(context, graphBackgroundColor());
//  CGContextFillRect(context, self.bounds);
  
  //CGContextTranslateCTM(context, 0.0, 56.0);
  
  
  // Draw the text
  UIFont *systemFont = [UIFont systemFontOfSize:22.0];
  [[UIColor whiteColor] set];
  [self.label drawInRect:CGRectMake(0.0, 0.0, 154.0, 26.0)
                         withFont:systemFont
                         lineBreakMode:UILineBreakModeWordWrap
                         alignment:UITextAlignmentRight];
  [floatToString(value, 2) drawInRect:CGRectMake(0.0, 20.0, 154.0, 26.0)
                           withFont:systemFont
                           lineBreakMode:UILineBreakModeWordWrap
                           alignment:UITextAlignmentRight];
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
