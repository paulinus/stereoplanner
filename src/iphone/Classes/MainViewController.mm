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
@synthesize selector;


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
  
  // Create the document
  doc_ = new SpDocument;
  	  
  captureViewController = [[CaptureViewController alloc] init];
  [captureViewController.view setFrame:[mama frame]];
  [captureViewController setDocument:doc_];
  
  cinemaViewController = [[CinemaViewController alloc] init]; 
  [cinemaViewController.view setFrame:[mama frame]];
  [cinemaViewController setDocument:doc_];

  [self selectSetCinema];
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
      [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromRight forView:mama cache:YES];
      [cinemaViewController.view removeFromSuperview];
      [mama addSubview:captureViewController.view];
      break;
    case 1:
      [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromLeft forView:mama cache:YES];
      [captureViewController.view removeFromSuperview];
      [mama addSubview:cinemaViewController.view];
      break;
  }
  [UIView commitAnimations];
}

@end
