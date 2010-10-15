//
//  EAGLController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 7/7/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CaptureView.h"

class SpDocument;

@interface CaptureViewController : UIViewController {
  CaptureView *myview;
  
  SpDocument *doc_;
}

- (void)setDocument:(SpDocument *)document;

@end
