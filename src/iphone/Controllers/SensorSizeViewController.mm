//
//  SensorSizeViewController.mm
//  StereoPlanner
//
//  Created by Pau Gargallo on 11/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SensorSizeViewController.h"


@implementation SensorSizeViewController


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

  self.navigationItem.title = @"SensorSize";
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
    return 2;
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
  static NSString *CellIdentifier = @"Cell";
  
  UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
  if (cell == nil) {
    cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier] autorelease];
  }
  
  // Configure the cell...
  const SensorType &st = doc_->SensorTypeAt(indexPath.row);
  cell.textLabel.text = [NSString stringWithCString:st.label.c_str() encoding:[NSString defaultCStringEncoding]];
  cell.detailTextLabel.text = [NSString stringWithFormat:@"%gmm x %gmm", st.width * 1000, st.height * 1000];
  cell.accessoryType = (indexPath.row == doc_->SelectedSensorType()) ? UITableViewCellAccessoryCheckmark : UITableViewCellAccessoryNone;

  return cell;
}


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
  UITableViewCell *down = [tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:doc_->SelectedSensorType() inSection:indexPath.section]];
  if (down) down.accessoryType = UITableViewCellAccessoryNone;
  
  doc_->SetSelectedSensorType(indexPath.row);
  
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

