/*
 *  KinectNormal.h
 *  KinectBubble
 *
 *  Created by kikko on 11/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFbo.h"
#include "ofxShader.h"

class KinectNormal {

public:
	
	KinectNormal();
	~KinectNormal(){}
	
	void init(ofImage &depthImage);
	void update(ofImage &depthImage);
	void draw(int x, int y, int w, int h);
	
private:
	
	ofxShader normalMapShader;
	ofxFbo fbo;
};