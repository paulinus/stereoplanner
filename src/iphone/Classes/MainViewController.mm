//
//  StereoPlannerViewController.m
//  StereoPlanner
//
//  Created by Pau Gargallo on 6/24/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "MainViewController.h"

#include "document.h"


@implementation MainViewController

@synthesize mama;
@synthesize sliderSelectionView;
@synthesize selectObjectPicker;
@synthesize slider;
@synthesize selector;
@dynamic selectedSliderVariable;



/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
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

  CGRect mframe = [mama frame];
  mframe.origin.y = 0;
  captureViewController = [[CaptureViewController alloc] init];
  [captureViewController.view setFrame:mframe];
  [captureViewController setDocument:doc_];
  
  cinemaViewController = [[CinemaViewController alloc] init];
  [cinemaViewController.view setFrame:mframe];
  [cinemaViewController setDocument:doc_];
  
  [self selectSetCinema];
  
  [slider addTarget:self action:@selector(sliderChanged:) forControlEvents:UIControlEventValueChanged];
  [self setSelectedSliderVariable:SLIDER_CONVERGENCE];
   
  CGRect rect = mframe;
  int h = 40;
  rect.origin.y = rect.origin.y + rect.size.height - h;
  rect.size.height = h;
}

- (void)setDocument:(SpDocument *)document {
  doc_ = document;
  [captureViewController setDocument:doc_];
  [cinemaViewController setDocument:doc_];
}

-(int)selectedObject {
  return selectedObject;
}

-(void)setSelectedObject:(int)s {
  selectedObject = s;
  [(CaptureView *)captureViewController.view setSelectedObject:selectedObject];
}


- (SliderVariable)selectedSliderVariable {
  return selectedSliderVariable;
}

- (void)setSelectedSliderVariable:(SliderVariable)v {
  if (selectedSliderVariable != v) {
    selectedSliderVariable = v;
    [slider setLabel:[self getSliderVariableLabel]];
    [slider setValue:[self getSliderVariableValue]];
  }
}


- (NSString *)getSliderVariableLabel {
  switch (selectedSliderVariable) {
    case SLIDER_NEAR:
      return @"Near";
    case SLIDER_FAR:
      return @"Far";
    case SLIDER_CONVERGENCE:
      return @"Convergence";
    case SLIDER_FOCAL_LENGTH:
      return @"Focal length";
    case SLIDER_INTEROCULAR:
      return @"Interocular";
    case SLIDER_SCREEN_WIDTH:
      return @"Screen width";
    default:
      return @"";
  }
}

- (float)getSliderVariableValue {
  switch (selectedSliderVariable) {
    case SLIDER_NEAR:
      return doc_->NearDistance();
    case SLIDER_FAR:
      return doc_->FarDistance();
    case SLIDER_CONVERGENCE:
      return doc_->RigConvergence();
    case SLIDER_FOCAL_LENGTH:
      return doc_->FocalLegth();
    case SLIDER_INTEROCULAR:
      return doc_->RigInterocular();
    case SLIDER_SCREEN_WIDTH:
      return doc_->ScreenWidth();
    default:
      return 0;
  }
}

- (void)setSliderVariableValue:(float)value {
  switch (selectedSliderVariable) {
    case SLIDER_NEAR:
      doc_->SetNearDistance(value);
      break;
    case SLIDER_FAR:
      doc_->SetFarDistance(value);
      break;
    case SLIDER_CONVERGENCE:
      doc_->SetRigConvergence(value);
      break;
    case SLIDER_FOCAL_LENGTH:
      doc_->SetFocalLegth(value);
      break;
    case SLIDER_INTEROCULAR:
      doc_->SetRigInterocular(value);
      break;
    case SLIDER_SCREEN_WIDTH:
      doc_->SetScreenWidth(value);
      break;
  }
}

- (void)sliderChanged:(id)sender {
  if (slider.value != [self getSliderVariableValue]) {
    [self setSliderVariableValue:slider.value];
    [self documentChanged];
  }
}

- (void)documentChanged {
  if (slider.value != [self getSliderVariableValue])
    slider.value = [self getSliderVariableValue];
  
  if ([captureViewController.view isDescendantOfView:mama])
    [(CaptureView *)captureViewController.view updateGL];
  
  if ([cinemaViewController.view isDescendantOfView:mama])
    [(CinemaView *)cinemaViewController.view updateGL];
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
  return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

- (void)didReceiveMemoryWarning {
  // Releases the view if it doesn't have a superview.
  [super didReceiveMemoryWarning];
	
  // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
  // Release any retained subviews of the main view.
  // e.g. self.myOutlet = nil;
  delete doc_;
}

- (void)dealloc {
  [super dealloc];
}

- (IBAction)selectSetCinema {
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:0.75];
  
  switch ([selector selectedSegmentIndex]) {
    case 0:
      [(CaptureView *)captureViewController.view updateGL];
      [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromLeft forView:mama cache:YES];
      [cinemaViewController.view removeFromSuperview];
      [mama addSubview:captureViewController.view];
      break;
    case 1:     
      [(CinemaView *)cinemaViewController.view updateGL];
      [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromRight forView:mama cache:YES];
      [captureViewController.view removeFromSuperview];
      [mama addSubview:cinemaViewController.view];
      break;    
  }
  [UIView commitAnimations];
}


- (void)showSliderSelectionView {
  sliderSelectionView.alpha = 0;
  sliderSelectionView.hidden = NO;
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:0.15];
  sliderSelectionView.alpha = 1;
  [UIView commitAnimations];
}

- (void)hideSliderSelectionView {
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:0.15];
  sliderSelectionView.alpha = 0;
  [UIView commitAnimations];
}

- (IBAction)chooseSliderButton {
  if (sliderSelectionView.hidden == YES || sliderSelectionView.alpha == 0) {
    [self showSliderSelectionView];
  } else {    
    [self hideSliderSelectionView];
  }
}

- (IBAction)sliderSelectNear {
  self.selectedSliderVariable = SLIDER_NEAR;
  [self hideSliderSelectionView];
}
     
- (IBAction)sliderSelectFar {
  self.selectedSliderVariable = SLIDER_FAR;
  [self hideSliderSelectionView];
}
     
- (IBAction)sliderSelectConvergence {
  self.selectedSliderVariable = SLIDER_CONVERGENCE;
  [self hideSliderSelectionView];  
}
     
- (IBAction)sliderSelectInterocular {
  self.selectedSliderVariable = SLIDER_INTEROCULAR;
  [self hideSliderSelectionView];  
}
     
- (IBAction)sliderSelectFocalLength {
  self.selectedSliderVariable = SLIDER_FOCAL_LENGTH;
  [self hideSliderSelectionView];
}
     
- (IBAction)sliderSelectScreenWidth {
  self.selectedSliderVariable = SLIDER_SCREEN_WIDTH;
  [self hideSliderSelectionView];
}

- (IBAction)orbitButtonAction {
  [(CaptureView *)captureViewController.view setInteractionMode:CaptureViewInteractionModeOrbit];
}

- (IBAction)moveButtonAction {
  [(CaptureView *)captureViewController.view setInteractionMode:CaptureViewInteractionModeMove];
}

- (void)showSelectObjectView {
  selectObjectPicker.alpha = 0;
  selectObjectPicker.hidden = NO;
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:0.15];
  selectObjectPicker.alpha = 1;
  [UIView commitAnimations];
}

- (void)hideSelectObjectView {
  [UIView beginAnimations:nil context:NULL];
  [UIView setAnimationDuration:0.15];
  selectObjectPicker.alpha = 0;
  [UIView commitAnimations];
}

- (IBAction)selectButtonAction {
  if (selectObjectPicker.hidden == YES || selectObjectPicker.alpha == 0) {
    [self showSelectObjectView];
  } else {    
    [self hideSelectObjectView];
  }
}

- (IBAction)addButtonAction {
  NSString *filePath = [[NSBundle mainBundle] pathForResource:@"monkey" ofType:@"geo"];  
  if (filePath) {
    doc_->AddObject([filePath cStringUsingEncoding:1]);
  }
}

- (IBAction)removeButtonAction {
  doc_->RemoveObject(selectedObject);
}

#pragma mark -
#pragma mark UIPickerView stuff

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
  return 1;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
  [self setSelectedObject:row];
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
  return doc_->scene_.children_.size();
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
  return [NSString stringWithCString:doc_->scene_.children_[row]->name_.c_str() 
                   encoding:[NSString defaultCStringEncoding]];
}



@end
