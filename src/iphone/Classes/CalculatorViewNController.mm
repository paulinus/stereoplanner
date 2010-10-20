//
//  CalculatorViewNController.mm
//  StereoPlanner
//
//  Created by Pau Gargallo on 9/8/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "document.h"
#import "utils.h"
#import "MainViewController.h"
#import "CalculatorViewNController.h"


@implementation CalculatorViewNController

@synthesize nearLabel;
@synthesize farLabel;
@synthesize convergenceLabel;
@synthesize focalLengthLabel;
@synthesize interocularLabel;
@synthesize screenWidthLabel;
@synthesize nearParallaxLabel;
@synthesize farParallaxLabel;
@synthesize parallaxBudgedLabel;
@synthesize nearScreenParallaxLabel;
@synthesize farScreenParallaxLabel;
@synthesize screenParallaxBudgedLabel;


- (id)initWithMainViewController:(MainViewController *)mvc {
  main_view_controller_ = mvc;
  return [self init];
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

- (void)setDocument:(SpDocument *)document {
  doc_ = document;
  [self updateView];
}

- (IBAction)updateView {
  [nearLabel setText:floatToStringInMetric(doc_->NearDistance(), 2)];
  [farLabel setText:floatToStringInMetric(doc_->FarDistance(), 2)];
  [convergenceLabel setText:floatToStringInMetric(doc_->RigConvergence(), 2)];
  [focalLengthLabel setText:floatToStringInMetric(doc_->FocalLegth(), 2)];
  [interocularLabel setText:floatToStringInMetric(doc_->RigInterocular(), 2)];
  [screenWidthLabel setText:floatToStringInMetric(doc_->ScreenWidth(), 2)];
  [nearParallaxLabel setText:floatToStringPercentage(doc_->NearParallax(), 2)];
  [farParallaxLabel setText:floatToStringPercentage(doc_->FarParallax(), 2)];
  [parallaxBudgedLabel setText:floatToStringPercentage(doc_->ParallaxBudged(), 2)];
  [nearScreenParallaxLabel setText:floatToStringInMetric(doc_->NearScreenParallax(), 2)];
  [farScreenParallaxLabel setText:floatToStringInMetric(doc_->FarScreenParallax(), 2)];
  [screenParallaxBudgedLabel setText:floatToStringInMetric(doc_->ScreenParallaxBudged(), 2)];
}

- (IBAction)nearButton {
  [main_view_controller_ setSliderVariableSelection:SLIDER_NEAR];
}

- (IBAction)farButton {
  [main_view_controller_ setSliderVariableSelection:SLIDER_FAR]; 
}

- (IBAction)convergenceButton {
  [main_view_controller_ setSliderVariableSelection:SLIDER_CONVERGENCE];
}

- (IBAction)focalLengthButton {
  [main_view_controller_ setSliderVariableSelection:SLIDER_FOCAL_LENGTH];
}

- (IBAction)interocularButton {
  [main_view_controller_ setSliderVariableSelection:SLIDER_INTEROCULAR];
}

- (IBAction)screenWidthButton {
  [main_view_controller_ setSliderVariableSelection:SLIDER_SCREEN_WIDTH];
}


@end
