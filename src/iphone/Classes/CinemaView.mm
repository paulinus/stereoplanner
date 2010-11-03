//
//  EAGLView.m
//  provaOpenGLES
//
//  Created by Pau Gargallo on 6/26/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#include "document.h"

#import "CinemaView.h"


@implementation CinemaView

- (void)setDocument:(const SpDocument *)document {
  
  doc_ = document;
  [self updateGL];
}

- (void)drawViewingAreaAtDepth:(float)z {
  float lleft, lright, rleft, rright, bottom, top;
  StereoFrustum f = doc_->ViewingFrustrum();
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

- (void)draw {
  [super draw];
  
  if (doc_) {
    [super renderGeometry:&(doc_->TheaterGeometry())];
    
    // Draw the observer.
    Vector3f posl = doc_->EyePosition(0);
    Vector3f posr = doc_->EyePosition(1);
    GLfloat eyes[] = {
      doc_->ObserverX(), doc_->ObserverY(), doc_->ObserverZ(),
      posl[0], posl[1], posl[2],
      posr[0], posr[1], posr[2]
    };
    GLfloat eyecolors[] = {
      0, 1, 0, 1,
      1., .7, .5, 1,
      .5, .7, 1., 1
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
    float w = doc_->ScreenWidth() / 2;
    float h = doc_->ScreenHeight() / 2;
    float z = 0;
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
  }  
}


- (void)dealloc {
  
  [super dealloc];
}

@end
