//
//  StereoPlannerAppDelegate.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 6/24/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CalculatorViewController;
class SpDocument;

@interface StereoPlannerAppDelegate : NSObject <UIApplicationDelegate> {
  UIWindow *window;
  CalculatorViewController *viewController;
  SpDocument *doc_;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet CalculatorViewController *viewController;

@end

