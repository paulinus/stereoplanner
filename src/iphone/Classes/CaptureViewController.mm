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
  
  myview = [[EAGLView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
  
  self.view = myview;
}

- (void)setDocument:(SpDocument *)document {
  doc_ = document;
  [myview setGeometry:nil];
  [myview setGeometry:&(doc_->CaptureGeometry())];
}


@end
