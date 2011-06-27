//
//  CalculatorViewController.mm
//  StereoPlanner
//
//  Created by Pau Gargallo on 9/8/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "document.h"
#import "utils.h"
#import "MainViewController.h"
#import "SettingsViewController.h"
#import "CalculatorViewController.h"


@implementation CalculatorViewController

@synthesize main_view_controller_;

@synthesize nearParallaxLabel;
@synthesize farParallaxLabel;
@synthesize parallaxBudgedLabel;
@synthesize nearScreenParallaxLabel;
@synthesize farScreenParallaxLabel;
@synthesize screenParallaxBudgedLabel;
@synthesize maxInterocularLabel;

@synthesize nearSlider;
@synthesize farSlider;
@synthesize convergenceSlider;
@synthesize interocularSlider;
@synthesize focalLengthSlider;
@synthesize screenWidthSlider;


- (id)initWithCoder:(NSCoder *)coder {
  self = [super initWithCoder:coder];
  if (self) {
    isShowingLandscapeView = NO;

    self.main_view_controller_ = [[MainViewController alloc] init];

    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(orientationChanged:)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];

  }
  return self;
}

- (void)orientationChanged:(NSNotification *)notification {
  UIDeviceOrientation deviceOrientation = [UIDevice currentDevice].orientation;
  if (UIDeviceOrientationIsLandscape(deviceOrientation) &&
      !isShowingLandscapeView) {
    [main_view_controller_ documentChanged];
    [self presentModalViewController:self.main_view_controller_ animated:YES];
    isShowingLandscapeView = YES;
  } else if (UIDeviceOrientationIsPortrait(deviceOrientation) &&
           isShowingLandscapeView) {
    [self documentChanged];
    [self dismissModalViewControllerAnimated:YES];
    isShowingLandscapeView = NO;
  }
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
    [self documentChanged];
  }
}

- (void)farSliderChanged:(id)sender {
  if (farSlider.value != doc_->FarDistance()) {
    doc_->SetFarDistance(farSlider.value);
    [self documentChanged];
  }
}

- (void)convergenceSliderChanged:(id)sender {
  if (convergenceSlider.value != doc_->RigConvergence()) {
    doc_->SetRigConvergence(convergenceSlider.value);
    [self documentChanged];
  }
}

- (void)interocularSliderChanged:(id)sender {
  if (interocularSlider.value != doc_->RigInterocular()) {
    doc_->SetRigInterocular(interocularSlider.value);
    [self documentChanged];
  }
}

- (void)focalLengthSliderChanged:(id)sender {
  if (focalLengthSlider.value != doc_->FocalLegth()) {
    doc_->SetFocalLegth(focalLengthSlider.value);
    [self documentChanged];
  }
}

- (void)screenWidthSliderChanged:(id)sender {
  if (screenWidthSlider.value != doc_->ScreenWidth()) {
    doc_->SetScreenWidth(screenWidthSlider.value);
    [self documentChanged];
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
  [main_view_controller_ setDocument:doc_];
  [self updateView];
}

- (IBAction)updateView {
  // Update sliders.
  [nearSlider setValue:doc_->NearDistance()];
  [farSlider setValue:doc_->FarDistance()];
  [convergenceSlider setValue:doc_->RigConvergence()];
  [interocularSlider setValue:doc_->RigInterocular()];
  [focalLengthSlider setValue:doc_->FocalLegth()];
  [screenWidthSlider setValue:doc_->ScreenWidth()];
  
  // Update parallax labels.
  [nearParallaxLabel setText:floatToStringPercentage(100 * doc_->NearParallax(), 2)];
  if (doc_->NearParallax() > doc_->MinParallaxConstraint()) {
    [nearParallaxLabel setTextColor:[UIColor greenColor]];
  } else {
    [nearParallaxLabel setTextColor:[UIColor redColor]];
  }
  
  [farParallaxLabel setText:floatToStringPercentage(100 * doc_->FarParallax(), 2)];
  if (doc_->FarParallax() < doc_->MaxParallaxConstraint()) {
    [farParallaxLabel setTextColor:[UIColor greenColor]];
  } else {
    [farParallaxLabel setTextColor:[UIColor redColor]];
  }
  
  [parallaxBudgedLabel setText:floatToStringPercentage(100 * doc_->ParallaxBudged(), 2)];
  if (doc_->ParallaxBudged() < doc_->MaxBracketConstraint()) {
    [parallaxBudgedLabel setTextColor:[UIColor greenColor]];
  } else {
    [parallaxBudgedLabel setTextColor:[UIColor redColor]];
  }
  
  
  
  [nearScreenParallaxLabel setText:floatToStringInMetric(doc_->NearScreenParallax(), 2)];
  [farScreenParallaxLabel setText:floatToStringInMetric(doc_->FarScreenParallax(), 2)];
  [screenParallaxBudgedLabel setText:floatToStringInMetric(doc_->ScreenParallaxBudged(), 2)];
  [maxInterocularLabel setText:floatToStringInMetric(doc_->MaxRigInterocular(), 2)];
}

- (void)documentChanged {
  [self updateView];
}

- (IBAction)showSettings {
  SettingsViewController *s = [[SettingsViewController alloc] init];
  UINavigationController *n = [[UINavigationController alloc] initWithRootViewController:s];
  [self presentModalViewController:n animated:YES];
  [n release];
  [s release];
}


@end
