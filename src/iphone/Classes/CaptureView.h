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
  int selectedObject;

  SpDocument *doc_; //TODO(pau) make this const
}

@property (nonatomic) CaptureViewInteractionMode interactionMode;
@property int selectedObject;

- (void)setDocument:(SpDocument *)document;
- (void)draw;

@end
