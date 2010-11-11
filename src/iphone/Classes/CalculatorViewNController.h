//
//  CalculatorViewNController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 9/8/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

class SpDocument;
@class MainViewController;

@interface CalculatorViewNController : UIViewController {
  SpDocument *doc_;
  MainViewController *main_view_controller_;
  
  id nearLabel;
  id farLabel;
  id convergenceLabel;
  id focalLengthLabel;
  id interocularLabel;
  id screenWidthLabel;
  id nearParallaxLabel;
  id farParallaxLabel;
  id parallaxBudgedLabel;
  id nearScreenParallaxLabel;
  id farScreenParallaxLabel;
  id screenParallaxBudgedLabel;
  
  InfiniteSlider *nearSlider;
  InfiniteSlider *farSlider;
  InfiniteSlider *convergenceSlider;
  InfiniteSlider *interocularSlider;
  InfiniteSlider *focalLengthSlider;
  InfiniteSlider *screenWidthSlider;
}

@property (nonatomic,assign) IBOutlet id nearLabel;
@property (nonatomic,assign) IBOutlet id farLabel;
@property (nonatomic,assign) IBOutlet id convergenceLabel;
@property (nonatomic,assign) IBOutlet id focalLengthLabel;
@property (nonatomic,assign) IBOutlet id interocularLabel;
@property (nonatomic,assign) IBOutlet id screenWidthLabel;
@property (nonatomic,assign) IBOutlet id nearParallaxLabel;
@property (nonatomic,assign) IBOutlet id farParallaxLabel;
@property (nonatomic,assign) IBOutlet id parallaxBudgedLabel;
@property (nonatomic,assign) IBOutlet id nearScreenParallaxLabel;
@property (nonatomic,assign) IBOutlet id farScreenParallaxLabel;
@property (nonatomic,assign) IBOutlet id screenParallaxBudgedLabel;

@property (nonatomic,assign) IBOutlet InfiniteSlider *nearSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *farSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *convergenceSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *interocularSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *focalLengthSlider;
@property (nonatomic,assign) IBOutlet InfiniteSlider *screenWidthSlider;



- (id)initWithMainViewController:(MainViewController *)mvc;
- (void)setDocument:(SpDocument *)document;
- (void)highlightSelection;
- (IBAction)updateView;
- (IBAction)nearButton;
- (IBAction)farButton;
- (IBAction)convergenceButton;
- (IBAction)focalLengthButton;
- (IBAction)interocularButton;
- (IBAction)screenWidthButton;
- (void)nearSliderChanged:(id)sender;


@end
