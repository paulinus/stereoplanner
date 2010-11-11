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

@synthesize nearSlider;
@synthesize farSlider;
@synthesize convergenceSlider;
@synthesize interocularSlider;
@synthesize focalLengthSlider;
@synthesize screenWidthSlider;


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

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
  [super viewDidLoad];
  [nearSlider setLabel:@"Near"];
  [farSlider setLabel:@"Far"];
  [convergenceSlider setLabel:@"Convergence"];
  [interocularSlider setLabel:@"Interocular"];
  [focalLengthSlider setLabel:@"Focal Length"];
  [screenWidthSlider setLabel:@"Screen Width"];
  [nearSlider addTarget:self action:@selector(nearSliderChanged:) forControlEvents:UIControlEventValueChanged];
  [farSlider addTarget:self action:@selector(farSliderChanged:) forControlEvents:UIControlEventValueChanged];
  [convergenceSlider addTarget:self action:@selector(convergenceSliderChanged:) forControlEvents:UIControlEventValueChanged];
  [interocularSlider addTarget:self action:@selector(interocularSliderChanged:) forControlEvents:UIControlEventValueChanged];
  [focalLengthSlider addTarget:self action:@selector(focalLengthSliderChanged:) forControlEvents:UIControlEventValueChanged];
  [screenWidthSlider addTarget:self action:@selector(screenWidthSliderChanged:) forControlEvents:UIControlEventValueChanged];
}

- (void)nearSliderChanged:(id)sender {
  if (nearSlider.value != doc_->NearDistance()) {
    doc_->SetNearDistance(nearSlider.value);
    [self updateView];
  }
}

- (void)farSliderChanged:(id)sender {
  if (farSlider.value != doc_->FarDistance()) {
    doc_->SetFarDistance(farSlider.value);
    [self updateView];
  }
}

- (void)convergenceSliderChanged:(id)sender {
  if (convergenceSlider.value != doc_->RigConvergence()) {
    doc_->SetRigConvergence(convergenceSlider.value);
    [self updateView];
  }
}

- (void)interocularSliderChanged:(id)sender {
  if (interocularSlider.value != doc_->RigInterocular()) {
    doc_->SetRigInterocular(interocularSlider.value);
    [self updateView];
  }
}

- (void)focalLengthSliderChanged:(id)sender {
  if (focalLengthSlider.value != doc_->FocalLegth()) {
    doc_->SetFocalLegth(focalLengthSlider.value);
    [self updateView];
  }
}

- (void)screenWidthSliderChanged:(id)sender {
  if (screenWidthSlider.value != doc_->ScreenWidth()) {
    doc_->SetScreenWidth(screenWidthSlider.value);
    [self updateView];
  }
}



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
  [nearSlider setValue:doc_->NearDistance()];
  [farSlider setValue:doc_->FarDistance()];
  [convergenceSlider setValue:doc_->RigConvergence()];
  [interocularSlider setValue:doc_->RigInterocular()];
  [focalLengthSlider setValue:doc_->FocalLegth()];
  [screenWidthSlider setValue:doc_->ScreenWidth()];

  [self highlightSelection];
  [nearLabel setText:floatToStringInMetric(doc_->NearDistance(), 2)];
  [farLabel setText:floatToStringInMetric(doc_->FarDistance(), 2)];
  [convergenceLabel setText:floatToStringInMetric(doc_->RigConvergence(), 2)];
  [focalLengthLabel setText:floatToStringInMetric(doc_->FocalLegth(), 2)];
  [interocularLabel setText:floatToStringInMetric(doc_->RigInterocular(), 2)];
  [screenWidthLabel setText:floatToStringInMetric(doc_->ScreenWidth(), 2)];
  [nearParallaxLabel setText:floatToStringPercentage(100 * doc_->NearParallax(), 2)];
  [farParallaxLabel setText:floatToStringPercentage(100 * doc_->FarParallax(), 2)];
  [parallaxBudgedLabel setText:floatToStringPercentage(100 * doc_->ParallaxBudged(), 2)];
  [nearScreenParallaxLabel setText:floatToStringInMetric(doc_->NearScreenParallax(), 2)];
  [farScreenParallaxLabel setText:floatToStringInMetric(doc_->FarScreenParallax(), 2)];
  [screenParallaxBudgedLabel setText:floatToStringInMetric(doc_->ScreenParallaxBudged(), 2)];
}

- (void)highlightSelection {
 UIColor *a = [UIColor blueColor];
  UIColor *b = [UIColor clearColor];
  [nearLabel setBackgroundColor:
   ([main_view_controller_ selectedSliderVariable] == SLIDER_NEAR) ? a : b];
  [farLabel setBackgroundColor:
   ([main_view_controller_ selectedSliderVariable] == SLIDER_FAR) ? a : b];
  [convergenceLabel setBackgroundColor:
   ([main_view_controller_ selectedSliderVariable] == SLIDER_CONVERGENCE) ? a : b];
  [focalLengthLabel setBackgroundColor:
   ([main_view_controller_ selectedSliderVariable] == SLIDER_FOCAL_LENGTH) ? a : b];
  [interocularLabel setBackgroundColor:
   ([main_view_controller_ selectedSliderVariable] == SLIDER_INTEROCULAR) ? a : b];
  [screenWidthLabel setBackgroundColor:
   ([main_view_controller_ selectedSliderVariable] == SLIDER_SCREEN_WIDTH) ? a : b];
}

- (IBAction)nearButton {
  [main_view_controller_ setSelectedSliderVariable:SLIDER_NEAR];
}

- (IBAction)farButton {
  [main_view_controller_ setSelectedSliderVariable:SLIDER_FAR]; 
}

- (IBAction)convergenceButton {
  [main_view_controller_ setSelectedSliderVariable:SLIDER_CONVERGENCE];
}

- (IBAction)focalLengthButton {
  [main_view_controller_ setSelectedSliderVariable:SLIDER_FOCAL_LENGTH];
}

- (IBAction)interocularButton {
  [main_view_controller_ setSelectedSliderVariable:SLIDER_INTEROCULAR];
  [self highlightSelection];
}

- (IBAction)screenWidthButton {
  [main_view_controller_ setSelectedSliderVariable:SLIDER_SCREEN_WIDTH];
}


@end
