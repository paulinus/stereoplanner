//
//  EAGLView.h
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface InfiniteSlider : UIControl {
  float value;
  NSString *label;
}

@property (nonatomic) float value;
@property (assign,nonatomic) NSString *label;


@end
