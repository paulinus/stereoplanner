//
//  SensorSizeViewController.h
//  StereoPlanner
//
//  Created by Pau Gargallo on 11/21/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "document.h"

#import <UIKit/UIKit.h>


@interface SensorSizeViewController : UITableViewController {
  SpDocument *doc_;
}

- (id)initWithDocument:(SpDocument *)document;

@end
