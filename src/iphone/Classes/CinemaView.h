//
//  EAGLView.h
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


#import "EAGLView.h"

class SpDocument;


@interface CinemaView : EAGLView {
@private
  const SpDocument *doc_;
}

- (void)setDocument:(const SpDocument *)document;
- (void)draw;

@end
