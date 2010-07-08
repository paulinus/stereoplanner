//
//  EAGLView.h
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#include "trackball.h"

class Geometry;


@interface EAGLView : UIView {
  float rotation;
@private
  Trackball trackball_;
  const Geometry *geometry_;
  
  EAGLContext *context;
  
  // The pixel dimensions of the CAEAGLLayer.
  GLint backingWidth;
  GLint backingHeight;
  
  // The OpenGL ES buffer names.
  GLuint viewRenderbuffer;
  GLuint viewFramebuffer;
  GLuint depthRenderbuffer;
}

@property (nonatomic) float rotation;

- (void)setGeometry:(const Geometry *)geometry;
- (void)drawView;
- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;


@end
