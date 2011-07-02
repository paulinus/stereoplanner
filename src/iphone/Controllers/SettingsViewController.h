//
//  SettingsViewController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 11/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SettingsViewController : UITableViewController {
  SpDocument *doc_;
}

- (id)initWithDocument:(SpDocument *)document;
- (void)doneButton;
@end
