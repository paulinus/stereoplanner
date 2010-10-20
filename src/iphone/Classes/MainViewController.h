//
//  StereoPlannerViewController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 6/24/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "InfiniteSlider.h"
#import "CaptureViewController.h"
#import "CinemaViewController.h"
#import "CalculatorViewNController.h"

class SpDocument;

typedef enum {
  SLIDER_NEAR,
  SLIDER_FAR,
  SLIDER_CONVERGENCE,
  SLIDER_FOCAL_LENGTH,
  SLIDER_INTEROCULAR,
  SLIDER_SCREEN_WIDTH
} SliderVariable;

@interface MainViewController : UIViewController {
  UIView *mama;
  CaptureViewController *captureViewController;
  CinemaViewController *cinemaViewController;
  CalculatorViewNController *calculatorViewController;

  InfiniteSlider *slider;
  
  SliderVariable selected_slider_variable;

  UISegmentedControl *selector;
  SpDocument *doc_;
}

@property (nonatomic,assign) IBOutlet UIView *mama;
@property (nonatomic,assign) IBOutlet UISegmentedControl *selector;


- (void)setSliderVariableSelection:(SliderVariable)v;
- (NSString *)getSliderVariableLabel;
- (float)getSliderVariableValue;
- (void)setSliderVariableValue:(float)value;      
- (IBAction)selectSetCinema;

@end

