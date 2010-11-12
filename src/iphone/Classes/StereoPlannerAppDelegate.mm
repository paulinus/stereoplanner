//
//  StereoPlannerAppDelegate.m
//  StereoPlanner
//
//  Created by Pau Gargallo on 6/24/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "StereoPlannerAppDelegate.h"
#import "CalculatorViewNController.h"
#include "document.h"

@implementation StereoPlannerAppDelegate

@synthesize window;
@synthesize viewController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    

  // Override point for customization after app launch    
  [window addSubview:viewController.view];
  [window makeKeyAndVisible];
 
  // Create the document
  doc_ = new SpDocument; 
  [viewController setDocument:doc_];

  return YES;
}


- (void)dealloc {
  [viewController release];
  [window release];
  delete doc_;
  [super dealloc];
}


@end
