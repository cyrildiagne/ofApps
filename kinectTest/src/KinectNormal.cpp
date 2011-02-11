/*
 *  KinectNormal.cpp
 *  KinectBubble
 *
 *  Created by kikko on 11/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "KinectNormal.h"

KinectNormal::KinectNormal(){
	
	
}

void KinectNormal::init(ofImage &depthImage){
	
	fbo.setup(depthImage.width, depthImage.height, GL_RGBA, 0);
	normalMapShader.setup("shaders/normalMapping.vert", "shaders/normalMapping.frag");
}

void KinectNormal::update(ofImage &depthImage){
	
	fbo.begin();
	normalMapShader.begin();
	
	depthImage.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	
	normalMapShader.end();
	fbo.end();
}

void KinectNormal::draw(int x, int y, int w, int h){
	
	fbo.draw(x, y, w, h);
}