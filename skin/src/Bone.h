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
#include "ofxOpenNI.h"

class Bone : public ofNode {
	
public:
	
	Bone();
	
	void init(XnSkeletonJoint xnJointId, ofQuaternion initRotate);
	void updateCalibPose();
	void customDraw();
	
	ofVec3f getPositionAlong(float percent);
	
	ofMatrix4x4 bindPose;
	ofMatrix4x4 calibPose;
	float length;
	ofColor debugColor;
	
	XnSkeletonJoint xnJointId;
	
private:
	ofNode debugNode;
};