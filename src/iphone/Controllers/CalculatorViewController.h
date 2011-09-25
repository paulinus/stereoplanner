//
//  CalculatorViewController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 9/8/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "InfiniteSlider.h"
#import "FlipsideViewController.h"

class SpDocument;
@class MainViewController;

@interface CalculatorViewController : UIViewController <FlipsideViewControllerDelegate> {
  SpDocument *doc_;
  MainViewController *main_view_controller_;
  
  bool isShowingLandscapeView;
  
  id nearParallaxLabel;
  id farParallaxLabel;
  id parallaxBudgedLabel;
  id nearScreenParallaxLabel;
  id farScreenParallaxLabel;
  id screenParallaxBudgedLabel;
  id roundnessLabel;
  id maxInterocularLabel;
  
  InfiniteSlider *nearSlider;
  InfiniteSlider *farSlider;
  InfiniteSlider *convergenceSlider;
  InfiniteSlider *interocularSlider;
  InfiniteSlider *focalLengthSlider;
  InfiniteSlider *screenWidthSlider;
}

@property (nonatomic,assign) MainViewController *main_view_controller_;

@property (nonatomic,assign) IBOutlet id nearParallaxLabel;
@property (nonatomic,assign) IBOutlet id farParallaxLabel;
@property (nonatomic,assign) IBOutlet id parallaxBudgedLabel;
@property (nonatomic,assign) IBOutlet id nearScreenParallaxLabel;
@property (nonatomic,assign) IBOutlet id farScreenParallaxLabel;
@property (nonatomic,assign) IBOutlet id screenParallaxBudgedLabel;
@property (nonatomic,assign) IBOutlet id roundnessLabel;
@property (nonatomic,assign) IBOutlet id maxInterocularLabel;

@property (nonatomic,assign) IBOutlet InfiniteSlider *nearSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *farSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *convergenceSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *interocularSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *focalLengthSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *screenWidthSlider;



- (void)setDocument:(SpDocument *)document;
- (IBAction)updateView;
- (void)documentChanged;
- (IBAction)showSettings;
- (IBAction)showInfo;


@end
