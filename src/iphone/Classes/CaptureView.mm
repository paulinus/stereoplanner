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

- (void)setDocument:(const SpDocument *)document {
  
  doc_ = document;
  [self updateGL];
}


- (void)draw {
  [super draw];

  if (doc_) {
    [super renderGeometry:&(doc_->CaptureGeometry())];
  
    
    glPushMatrix(); // Move to rig's reference frame.
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
    float w = doc_->RigConvergence() * doc_->SensorWidth() 
              / doc_->FocalLegth() / 2;
    float h = doc_->RigConvergence() * doc_->SensorHeight()
              / doc_->FocalLegth() / 2;
    float z = -doc_->RigConvergence();
  
    GLfloat screen[] = {
      -w, -h, z,  +w, -h, z,
      +w, -h, z,  +w, +h, z,
      +w, +h, z,  -w, +h, z,
      -w, +h, z,  -w, -h, z
    };
    glDisable(GL_LIGHTING);
    glColor4f(.7, .7, .7, 1);
    glVertexPointer(3, GL_FLOAT, 0, screen);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES, 0, 8);
    glDisableClientState(GL_VERTEX_ARRAY);
   

    // Draw Frustum
    GLfloat frustruml[] = {
      l,0,0, -w, -h, z,
      l,0,0, +w, -h, z,
      l,0,0, +w, +h, z,
      l,0,0, -w, +h, z
    };
    GLfloat frustrumr[] = {
      r,0,0, -w, -h, z,
      r,0,0, +w, -h, z,
      r,0,0, +w, +h, z,
      r,0,0, -w, +h, z
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
    
    glPopMatrix(); // Rig's reference frame.
  }
}

- (void)dealloc {
  [super dealloc];
}

@end
