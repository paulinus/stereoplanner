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


- (id)initWithMainViewController:(MainViewController *)mvc;
- (void)setDocument:(SpDocument *)document;
- (IBAction)updateView;
- (IBAction)nearButton;
- (IBAction)farButton;
- (IBAction)convergenceButton;
- (IBAction)focalLengthButton;
- (IBAction)interocularButton;
- (IBAction)screenWidthButton;


@end
