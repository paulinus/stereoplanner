//
//  EAGLView.m
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "EAGLView.h"

#include "document.h"


struct TwoTouchMoveDecomposition {
  CGPoint prevCenter, curCenter;
  CGPoint prevRadius, curRadius;
  
  CGPoint centerDisp;
  CGPoint radiusDisp;
  
  CGFloat normalDisp;
  CGFloat tangentDisp;
  CGFloat zoom;
  CGFloat rotation;
};

CGFloat distanceBetweenTwoPoints(CGPoint p1, CGPoint p2) {
  float x = p1.x - p2.x;
  float y = p1.y - p2.y;
  return sqrt(x * x + y * y);
}

CGPoint baricenter(CGPoint p1, CGPoint p2) {
  return CGPointMake((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}

CGPoint diff(CGPoint p1, CGPoint p2) {
  return CGPointMake(p1.x - p2.x, p1.y - p2.y);
}

CGFloat norm2(CGPoint p) {
  return p.x * p.x + p.y * p.y;
}

// Computes the length of the projection of d onto r.
CGFloat projection(CGPoint d, CGPoint r) {
  return (d.x * r.x + d.y * r.y) / norm2(r);
}


CGFloat angle(CGPoint from, CGPoint to) {
  return acos((from.x * to.x + from.y * to.y) / sqrt(norm2(from) * norm2(to)));
}


                   
void DecomposeTwoTouchMove(CGPoint prev1, CGPoint prev2,
                           CGPoint cur1, CGPoint cur2,
                           TwoTouchMoveDecomposition *d) {
  
  d->prevCenter = baricenter(prev1, prev2);
  d->curCenter = baricenter(cur1, cur2);
  d->prevRadius = diff(prev2, d->prevCenter);
  d->curRadius = diff(cur2, d->curCenter);
  
  d->centerDisp = diff(d->curCenter, d->prevCenter);
  d->radiusDisp = diff(d->curRadius, d->prevRadius);
  
  d->normalDisp = projection(d->radiusDisp, d->prevRadius);
  d->tangentDisp = sqrt(norm2(d->radiusDisp) - d->normalDisp * d->normalDisp);
  
  CGFloat normPrevRadius = norm2(d->prevRadius);
  if (normPrevRadius > 0.1) {
    d->zoom = sqrt(norm2(d->curRadius) / normPrevRadius);
  } else {
    d->zoom = 0;
  }
  d->rotation = angle(d->prevRadius, d->curRadius);
}




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
  
  [self setMultipleTouchEnabled:YES];
  
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
  
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  
  // Set up GL_LIGHT1
  GLfloat LightAmbient[]= { 0.3f, 0.3f, 0.4f, 1.0f }; 			
  GLfloat LightDiffuse[]= { 1.0f, 1.0f, 0.9f, 1.0f };				
  GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };				
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
  glEnable(GL_LIGHT1);
  
  
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
      *p++ = 0;
      *p++ = x0 + i;
      *p++ = -x0;
      *p++ = 0;
      *p++ = x0 + i;
    }
    for (int i = 0; i < n; ++i) {
      *p++ = x0 + i;
      *p++ = 0;
      *p++ = x0;
      *p++ = x0 + i;
      *p++ = 0;
      *p++ = -x0;
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
  if (geo->triangles_.size() > 0) {
    glEnable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, 0, &geo->vertex_[0]);
    
    if (geo->normal_.size() / 3 == geo->vertex_.size() / 4) {
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, 0, &geo->normal_[0]);
    }
    
    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0.5f, 0.5f, 8.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, geo->triangles_.size(), GL_UNSIGNED_SHORT,
                   &geo->triangles_[0]);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisable(GL_LIGHTING);
  }
  
  if (geo->lines_.size()) {
    glDisable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, 0, &geo->vertex_[0]);
    glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
    glDrawElements(GL_LINES, geo->lines_.size(), GL_UNSIGNED_SHORT,
                   &geo->lines_[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  
}

- (void)preDraw {
  // Make our opengl context current.
  [EAGLContext setCurrentContext:context];
  
  // Bind our framebuffer to draw into.
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glViewport(0, 0, backingWidth, backingHeight);
  
  trackball_.SetUpGlCamera();
  
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  
}

- (void)draw {
  //[self renderAxis];
  //[self renderGrid];
}

- (void)postDraw {
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void)updateGL {
  [self preDraw];
  [self draw];
  [self postDraw];
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
  
  [self updateGL];
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




- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  NSArray *allTouches = [[event allTouches] allObjects];
  
  if ([allTouches count] == 1) {
    UITouch *touch = [touches anyObject];
    
    CGPoint p1 = [touch previousLocationInView:self];
    CGPoint p2 = [touch locationInView:self];
    
    trackball_.MouseRevolveGravity(
                                   p1.x, p1.y, p2.x, p2.y);
    
  } else if ([allTouches count] == 2) {
    UITouch *t1 = [allTouches objectAtIndex:0];
    UITouch *t2 = [allTouches objectAtIndex:1];
    
    TwoTouchMoveDecomposition d;
    DecomposeTwoTouchMove([t1 previousLocationInView:self],
                          [t2 previousLocationInView:self],
                          [t1 locationInView:self],
                          [t2 locationInView:self],
                          &d);
    
      trackball_.MouseZoom(d.zoom);
      trackball_.MouseTranslate(d.prevCenter.x, d.prevCenter.y,
                                d.curCenter.x, d.curCenter.y);
  }
    
  
  [self updateGL];
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
