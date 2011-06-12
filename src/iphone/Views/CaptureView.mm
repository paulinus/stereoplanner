//
//  EAGLView.m
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#include "document.h"

#import "CaptureView.h"


@implementation CaptureView

@synthesize interactionMode;
@synthesize selectedObject;

- (id)commonInit {
  interactionMode = CaptureViewInteractionModeOrbit;
  return [super commonInit];
}

- (void)setDocument:(SpDocument *)document {
  
  doc_ = document;
  [self updateGL];
}

- (void)drawViewingAreaAtDepth:(float)z {
  float lleft, lright, rleft, rright, bottom, top;
  StereoFrustum f = doc_->ShootingFrustrum();
  f.ViewAreaLeft(z, &lleft, &lright, &bottom, &top);
  f.ViewAreaRight(z, &rleft, &rright, &bottom, &top);
  
  GLfloat z_view_area[] = {
    lleft, bottom, -z,  rleft, bottom, -z,
    rleft, bottom, -z,  lright, bottom, -z,
    lright, bottom, -z,  rright, bottom, -z,
    
    lleft, top, -z,  rleft, top, -z,
    rleft, top, -z,  lright, top, -z,
    lright, top, -z,  rright, top, -z,
    
    lleft, bottom, -z,  lleft, top, -z,
    rleft, bottom, -z,  rleft, top, -z,
    lright, bottom, -z,  lright, top, -z,
    rright, bottom, -z,  rright, top, -z
  };
  glDisable(GL_LIGHTING);
  glColor4f(.7, .7, .7, 1);
  glVertexPointer(3, GL_FLOAT, 0, z_view_area);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(GL_LINES, 0, 20);
  glDisableClientState(GL_VERTEX_ARRAY);
}

- (void)drawFrustumLines {
  float z = std::max(std::max(doc_->NearDistance(), doc_->RigConvergence()), 
                     doc_->FarDistance());
  float lleft, lright, rleft, rright, bottom, top;
  StereoFrustum f = doc_->ShootingFrustrum();
  f.ViewAreaLeft(z, &lleft, &lright, &bottom, &top);
  f.ViewAreaRight(z, &rleft, &rright, &bottom, &top);
  
  float l = -doc_->RigInterocular() / 2;
  
  GLfloat frustruml[] = {
    l,0,0, lleft, bottom, -z,
    l,0,0, lright, bottom, -z,
    l,0,0, lright, top, -z,
    l,0,0, lleft, top, -z
  };
  GLfloat frustrumr[] = {
    -l,0,0, rleft, bottom, -z,
    -l,0,0, rright, bottom, -z,
    -l,0,0, rright, top, -z,
    -l,0,0, rleft, top, -z
  };
  
  glDisable(GL_LIGHTING);
  glEnableClientState(GL_VERTEX_ARRAY);
  
  // left
  glColor4f(.7, .4, .4, 1);
  glVertexPointer(3, GL_FLOAT, 0, frustruml);
  glDrawArrays(GL_LINES, 0, 8);
  // right
  glColor4f(.4, .7, .7, 1);
  glVertexPointer(3, GL_FLOAT, 0, frustrumr);
  glDrawArrays(GL_LINES, 0, 8);
  glDisableClientState(GL_VERTEX_ARRAY);
}


// TODO(pau): split this function in subfunctions.
- (void)draw {
  [super draw];

  if (doc_) {
    float cyan[4] = { 0.9f, 0.7f, 0.0f, 1.0f };
    float blue[4] = { 0.5f, 0.5f, 8.0f, 1.0f };

    for (int i = 0; i < doc_->scene_.children_.size(); ++i) {
      Object *o = doc_->scene_.children_[i];
      glPushMatrix();
      Matrix3f R = o->orientation_.toRotationMatrix();
      Vector3f t = o->position_;
      Matrix4f T;
      T << R, t, MatrixXf::Zero(1,3), 1;
      glMultMatrixf(T.data());
      float *color = (selectedObject == i) ? cyan:blue;
      [super renderGeometry:&o->geometry_ withColor:color];
      glPopMatrix();
    }
  
    // Move to rig's reference frame.
    glPushMatrix();
    glTranslatef(doc_->RigX(), doc_->RigY(), doc_->RigZ());
    Transform3f m;
    m = PanTiltRollA(doc_->RigPan(), doc_->RigTilt(), doc_->RigRoll());
    glMultMatrixf(m.data());
    
    // Draw cameras.
    float l = -doc_->RigInterocular() / 2;
    float r = doc_->RigInterocular() / 2;
    GLfloat eyes[] = {
      0,0,0,
      l,0,0,
      r,0,0
    };
    GLfloat eyecolors[] = {
      0, 1, 0, 1,
      1, .7, .5, 1,
      .5, .7, 1, 1
    };
    
    glDisable(GL_LIGHTING);
    glPointSize(3);
    glVertexPointer(3, GL_FLOAT, 0, eyes);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, eyecolors);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glDrawArrays(GL_POINTS, 0, 3);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    
    // Draw the screen.
    [self drawViewingAreaAtDepth:doc_->RigConvergence()];
      
    // Draw Frustum
    [self drawFrustumLines];
    
    // Draw fear and far planes.
    [self drawViewingAreaAtDepth:doc_->NearDistance()];
    [self drawViewingAreaAtDepth:doc_->FarDistance()];
    
    glPopMatrix(); // Rig's reference frame.
  }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  if (interactionMode == CaptureViewInteractionModeOrbit) {
    [super touchesMoved:touches withEvent:event];
  } else if (interactionMode == CaptureViewInteractionModeMove
             && selectedObject >= 0
             && selectedObject < doc_->scene_.children_.size()) {
    NSArray *allTouches = [[event allTouches] allObjects];

    Object *so = doc_->scene_.children_[selectedObject];
    if ([allTouches count] == 1) {
      UITouch *touch = [touches anyObject];
      
      CGPoint p1 = [touch previousLocationInView:self];
      CGPoint p2 = [touch locationInView:self];
      
      Eigen::Vector3f p = trackball_.Project(so->position_);
      float depth = p[2];
      NSLog(@"u, v, depth = %g, %g, %g", p[0], p[1], depth);
      Eigen::Vector3f a = trackball_.BackProject(p1.x, p1.y, depth);
      Eigen::Vector3f b = trackball_.BackProject(p2.x, p2.y, depth);
      so->position_ += b - a;
    } 
  }
  doc_->UpdateEverything();
  [self updateGL];
}

- (void)dealloc {
  [super dealloc];
}

@end
