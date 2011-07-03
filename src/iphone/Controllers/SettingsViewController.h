//
//  SettingsViewController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 11/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@protocol SettingsDoneDelegate <NSObject>
  - (void)settingsDone;
@end


@interface SettingsViewController : UITableViewController {
  SpDocument *doc_;
  id delegate_;
}

- (id)initWithDocument:(SpDocument *)document delegate:(id)dele;
- (void)doneButton;
@end


