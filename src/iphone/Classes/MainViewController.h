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
  UIView *sliderSelectionView;
  CaptureViewController *captureViewController;
  CinemaViewController *cinemaViewController;
  CalculatorViewNController *calculatorViewController;

  InfiniteSlider *slider;
  
  SliderVariable selectedSliderVariable;

  UISegmentedControl *selector;
  SpDocument *doc_;
}

@property (nonatomic,assign) IBOutlet UIView *mama;
@property (nonatomic,assign) IBOutlet UIView *sliderSelectionView;
@property (nonatomic,assign) IBOutlet InfiniteSlider *slider;
@property (nonatomic,assign) IBOutlet UISegmentedControl *selector;
@property (nonatomic) SliderVariable selectedSliderVariable;


- (NSString *)getSliderVariableLabel;
- (float)getSliderVariableValue;
- (void)setSliderVariableValue:(float)value;      
- (IBAction)selectSetCinema;
- (void)setDocument:(SpDocument *)document;
- (void)documentChanged;

- (IBAction)chooseSliderButton;

- (IBAction)sliderSelectNear;
- (IBAction)sliderSelectFar;
- (IBAction)sliderSelectConvergence;
- (IBAction)sliderSelectInterocular;
- (IBAction)sliderSelectFocalLength;
- (IBAction)sliderSelectScreenWidth;

@end

