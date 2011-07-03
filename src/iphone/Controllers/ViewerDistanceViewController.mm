//
//  ViewerDistanceViewController.mm
//  StereoPlanner
//
//  Created by Pau Gargallo on 11/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ViewerDistanceViewController.h"

int num_ratios = 10;
float ratios[] = {.2, .5, 1., 1.5, 2, 3, 4, 5, 7.5, 10};

@implementation ViewerDistanceViewController


#pragma mark -
#pragma mark Initialization

- (id)initWithDocument:(SpDocument *)document {
  doc_ = document;
  return [self initWithStyle:UITableViewStylePlain];
}



#pragma mark -
#pragma mark View lifecycle

- (void)viewDidLoad {
  [super viewDidLoad];

  self.navigationItem.title = @"Viewer Distance";
  [self.navigationItem.leftBarButtonItem setTitle:@"Settings"];
}


#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return num_ratios;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
  static NSString *CellIdentifier = @"Cell";
  
  UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
  if (cell == nil) {
    cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
  }
  
  // Configure the cell...
  cell.textLabel.text = [NSString stringWithFormat:@"%g%s", ratios[indexPath.row] * 100, "% of screen width"];
  cell.accessoryType = (ratios[indexPath.row] == doc_->ObserverZRatio()) ? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;

  return cell;
}


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
  for (int i = 0; i < num_ratios; ++i) {
    UITableViewCell *down = [tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:i inSection:indexPath.section]];
    if (down) down.accessoryType = UITableViewCellAccessoryNone;
  }
  
  doc_->SetObserverZRatio(ratios[indexPath.row]);
  
  UITableViewCell *up = [tableView cellForRowAtIndexPath:indexPath];
  if (up) up.accessoryType = UITableViewCellAccessoryCheckmark;
}


#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}

- (void)dealloc {
    [super dealloc];
}


@end

