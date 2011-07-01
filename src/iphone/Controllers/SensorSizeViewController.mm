//
//  SensorSizeViewController.mm
//  StereoPlanner
//
//  Created by Pau Gargallo on 11/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SensorSizeViewController.h"


@implementation SensorSizeViewController

@synthesize selectedCell;


#pragma mark -
#pragma mark Initialization

- (id)init {
  return [self initWithStyle:UITableViewStylePlain];
}



#pragma mark -
#pragma mark View lifecycle

- (void)viewDidLoad {
  [super viewDidLoad];

  self.navigationItem.title = @"SensorSize";
  [self.navigationItem.leftBarButtonItem setTitle:@"Settings"];
  self.selectedCell = nil;
}


#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return 2;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
  static NSString *CellIdentifier = @"Cell";
  
  UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
  if (cell == nil) {
    cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
  }
  
  // Configure the cell...
  switch (indexPath.row) {
    case 0:
      cell.textLabel.text = @"Nikon FX";
      cell.accessoryType=UITableViewCellAccessoryNone;
      break;
    case 1:
      cell.textLabel.text = @"Nikon DX";
      cell.accessoryType=UITableViewCellAccessoryNone;
      break;
  }
  return cell;
}


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
  if (self.selectedCell) {
    UITableViewCell *down = [tableView cellForRowAtIndexPath:self.selectedCell];
    if (down) down.accessoryType = UITableViewCellAccessoryNone;
  }
  self.selectedCell = indexPath;
  UITableViewCell *up = [tableView cellForRowAtIndexPath:self.selectedCell];
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

