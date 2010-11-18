//
//  EAGLView.h
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//


#import "EAGLView.h"

class SpDocument;

typedef enum {
  CaptureViewInteractionModeOrbit,
  CaptureViewInteractionModeMove
} CaptureViewInteractionMode;


@interface CaptureView : EAGLView {
  CaptureViewInteractionMode interactionMode;

  const SpDocument *doc_;
}

@property (nonatomic) CaptureViewInteractionMode interactionMode;

- (void)setDocument:(const SpDocument *)document;
- (void)draw;

@end
