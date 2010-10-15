//
//  EAGLController.mm
//  StereoPlanner
//
//  Created by Pau Gargallo on 7/7/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "CaptureViewController.h"

#include "document.h"


@implementation CaptureViewController


- (void)loadView {
  self.wantsFullScreenLayout = NO;
  
  myview = [[CaptureView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
  
  self.view = myview;
  
  if (doc_) {
    [self setDocument:doc_];
  }
}

- (void)setDocument:(SpDocument *)document {
  doc_ = document;
  
  [myview setDocument:doc_];
}


@end
