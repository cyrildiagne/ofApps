/*
 *  Trail.h
 *  holler_logo_1
 *
 *  Created by kikko on 2/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofShape.h"
#include "Border.h"
#include "Assets.h"

class Trail {

public:
	
	Trail(ofPoint initPos, ofPoint initVelocity, Border* bounds, int letterId);
	
	void update();
		
	void draw();
	
	Border * bounds;
	
	ofPoint position;
	ofPoint velocity;
	ofPoint acc;
	
	ofColor color;
	
	int letterId;
	
private:
	
	ofPoint getNewTarget();
	ofPoint target;
};