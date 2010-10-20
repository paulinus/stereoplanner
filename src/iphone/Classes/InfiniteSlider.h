//
//  EAGLView.h
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
  ISmetric, ISpercentage
} ValueType;

@interface InfiniteSlider : UIControl {
  NSString *label;
  float value;
  ValueType valueType;
}

@property (nonatomic) float value;
@property (nonatomic) ValueType valueType;
@property (assign,nonatomic) NSString *label;


@end
