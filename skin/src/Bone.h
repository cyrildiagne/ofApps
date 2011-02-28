/*
 *  Bone.h
 *  skin
 *
 *  Created by kikko on 2/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class Bone : public ofNode {
	
public:
	
	Bone();
	
	void customDraw();
	
	ofVec3f getPositionAlong(float percent);
	
	ofMatrix4x4 bindPose;
	float length;
	ofColor debugColor;
	
private:
	ofNode debugNode;
};