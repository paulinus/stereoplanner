//
//  StereoPlannerViewController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 6/24/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CaptureViewController.h"
#import "CinemaViewController.h"

class SpDocument;

@interface MainViewController : UIViewController {
  UIView *mama;
  CaptureViewController *captureViewController;
  CinemaViewController *cinemaViewController;
  UISegmentedControl *selector;
  SpDocument *doc_;
}

@property (nonatomic,assign) IBOutlet UIView *mama;
@property (nonatomic,assign) IBOutlet UISegmentedControl *selector;

- (IBAction)selectSetCinema;


@end

