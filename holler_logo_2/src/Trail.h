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
	
	Trail(ofPoint initPos, ofPoint initVelocity);
	
	void update();
	void update(Border & bounds);
	void wander();
	ofPoint steer(ofPoint target);
	
	void draw();
	
	ofPoint position;
	ofPoint velocity;
	ofPoint acc;
	
	float r;
	float wandertheta;
	float maxforce;
	float maxspeed;
	
	ofColor color;
	
	int letterId;
	
private:
	
	bool bPlantMode;
	bool bActive;
	
	ofImage *particle;
	
	ofPoint steerVector;
	
	vector<ofPoint> pts;
	vector<float> angles;
	vector<ofColor> colors;
};