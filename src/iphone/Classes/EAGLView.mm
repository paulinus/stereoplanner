//
//  EAGLView.m
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "EAGLView.h"

#include "document.h"


@implementation EAGLView

@synthesize rotation;

// You must implement this method
+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (id)commonInit {
  viewFramebuffer = 0;
  viewRenderbuffer = 0;
  depthRenderbuffer = 0;
  geometry_ = 0;
  
  // Get the layer
  CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
  
  eaglLayer.opaque = YES;
  eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithBool:YES],
                                  kEAGLDrawablePropertyRetainedBacking,
                                  kEAGLColorFormatRGBA8,
                                  kEAGLDrawablePropertyColorFormat,
                                  nil];
  

  context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
  if (!context || ![EAGLContext setCurrentContext:context]) {
    [self release];
    return nil;
  }
  
  return self;
}

- (id)initWithFrame:(CGRect)frame {    
  if ((self = [super initWithFrame:frame])) {
    return [self commonInit];
  }
  return self;
}

- (id)initWithCoder:(NSCoder*)coder {    
  if ((self = [super initWithCoder:coder])) {
    return [self commonInit];
  }
  return self;
}


- (void)renderGrid {
  // Replace the implementation of this method to do your own custom drawing  
  static GLfloat *gridVertices = 0;
  static const int n = 10;
  
  if (!gridVertices) {
    gridVertices = (GLfloat *)malloc(sizeof(*gridVertices) * 2 * 6 * n);
    GLfloat *p = gridVertices;
    GLfloat x0 = - (n - 1) / 2.0f;
    for (int i = 0; i < n; ++i) {
      *p++ = x0;
      *p++ = x0 + i;
      *p++ = 0;
      *p++ = -x0;
      *p++ = x0 + i;
      *p++ = 0;
    }
    for (int i = 0; i < n; ++i) {
      *p++ = x0 + i;
      *p++ = x0;
      *p++ = 0;
      *p++ = x0 + i;
      *p++ = -x0;
      *p++ = 0;
    }
  }
  glVertexPointer(3, GL_FLOAT, 0, gridVertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
  glDrawArrays(GL_LINES, 0, 4 * n);
  glDisableClientState(GL_VERTEX_ARRAY);
}

- (void)renderAxis {
  // Replace the implementation of this method to do your own custom drawing  
  static const GLfloat axisVertices[] = {
    0, 0, 0,
    1, 0, 0,
    0, 0, 0,
    0, 1, 0,
    0, 0, 0,
    0, 0, 1,
  };
  
  static const GLfloat axisColors[] = {
    1,0,0,1,
    1,0,0,1,
    0,1,0,1,
    0,1,0,1,
    0,0,1,1,
    0,0,1,1,
  };
  
  glVertexPointer(3, GL_FLOAT, 0, axisVertices);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(4, GL_FLOAT, 0, axisColors);
  glEnableClientState(GL_COLOR_ARRAY);
  
  glDrawArrays(GL_LINES, 0, 6);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}


- (void)renderGeometry:(const Geometry *)geo {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(4, GL_FLOAT, 0,&geo->vertex_[0]);
  
  glColor4f(0.8f, 0.8f, 1.0f, 1.0f);
  glDrawElements(GL_TRIANGLES, geo->triangles_.size(), GL_UNSIGNED_SHORT, &geo->triangles_[0]);

  glDisableClientState(GL_VERTEX_ARRAY);
}


- (void)drawView {
  // Make our opengl context current.
  [EAGLContext setCurrentContext:context];
  
  // Bind our framebuffer to draw into.
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glViewport(0, 0, backingWidth, backingHeight);
  
  trackball_.SetUpGlCamera();
  
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  
  [self renderAxis];
  [self renderGrid];
  if (geometry_)
    [self renderGeometry:geometry_];

  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}



// If our view is resized, we'll be asked to layout subviews.
// This is the perfect opportunity to also update the framebuffer so that it is
// the same size as our display area.
-(void)layoutSubviews
{
  [EAGLContext setCurrentContext:context];
  [self destroyFramebuffer];
  [self createFramebuffer];
  
  trackball_.SetScreenSize(backingWidth, backingHeight);
  
  [self drawView];
}

- (BOOL)createFramebuffer {
  // Generate IDs for a framebuffer object and a color renderbuffer
  glGenFramebuffersOES(1, &viewFramebuffer);
  glGenRenderbuffersOES(1, &viewRenderbuffer);
  
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  
  // This call associates the storage for the current render buffer with the
  // EAGLDrawable (our CAEAGLLayer) allowing us to draw into a buffer that will
  // later be rendered to screen wherever the layer is (which corresponds with
  // our view).
  [context renderbufferStorage:GL_RENDERBUFFER_OES
                  fromDrawable:(id<EAGLDrawable>)self.layer];
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
                               GL_RENDERBUFFER_OES, viewRenderbuffer);
  
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
                                  GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
                                  GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
  
  // For this sample, we also need a depth buffer, so we'll create and attach
  // one via another renderbuffer.
  glGenRenderbuffersOES(1, &depthRenderbuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
  glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES,
                           backingWidth, backingHeight);
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
                               GL_RENDERBUFFER_OES, depthRenderbuffer);
  
  if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES)
      != GL_FRAMEBUFFER_COMPLETE_OES) {
    NSLog(@"failed to make complete framebuffer object %x",
          glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    return NO;
  }
  
  return YES;
}

// Clean up any buffers we have allocated.
- (void)destroyFramebuffer {
  glDeleteFramebuffersOES(1, &viewFramebuffer);
  viewFramebuffer = 0;
  glDeleteRenderbuffersOES(1, &viewRenderbuffer);
  viewRenderbuffer = 0;
  glDeleteRenderbuffersOES(1, &depthRenderbuffer);
  depthRenderbuffer = 0;
}


- (void)setGeometry:(const Geometry *)geometry {
  geometry_ = geometry;
  [self drawView];
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  UITouch *touch = [[event touchesForView:self] anyObject];
  
  CGPoint p1 = [touch previousLocationInView:self];
  CGPoint p2 = [touch locationInView:self];
  
  trackball_.MouseRevolve(p1.x, p1.y, p2.x, p2.y);
 
  [self drawView];
}








- (void)dealloc {
  // Tear down context
  if ([EAGLContext currentContext] == context)
    [EAGLContext setCurrentContext:nil];
  
  [context release];
  context = nil;
  
  [super dealloc];
}

@end
