//
//  EAGLController.mm
//  StereoPlanner
//
//  Created by Pau Gargallo on 7/7/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "CinemaViewController.h"

#include "document.h"


@implementation CinemaViewController


- (void)loadView {
  self.wantsFullScreenLayout = NO;
  
  myview = [[EAGLView alloc] initWithFrame:[UIScreen mainScreen].applicationFrame];
  
  self.view = myview;
  
  if (doc_) {
    [myview setGeometry:&(doc_->TheaterGeometry())];
  }
}

- (void)setDocument:(SpDocument *)document {
  doc_ = document;
  [myview setGeometry:&(doc_->TheaterGeometry())];
}


@end
