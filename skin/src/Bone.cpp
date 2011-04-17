/*
 *  Bone.cpp
 *  skin
 *
 *  Created by kikko on 2/24/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Bone.h"

Bone::Bone() {
	
	length = 300.f;
	debugColor.set(ofRandomf(), ofRandomf(), ofRandomf());
}

void Bone::init(XnSkeletonJoint xnJointId, ofQuaternion bindPoseRot) {
	
	this->xnJointId = xnJointId;
	
	bindPose.setRotate(bindPoseRot);
}

void Bone::updateCalibPose(){
	calibPose.makeInvertOf( getGlobalTransformMatrix() );
}

void Bone::customDraw() {
	
	ofSetColor(255, 0, 0);

	glPushMatrix();
	glTranslatef(0, -length/2, 0);
	glScalef(30, length, 30);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
}

ofVec3f Bone::getPositionAlong(float percent) {
	
	debugNode.setPosition(getPosition());
	debugNode.setOrientation(getOrientationQuat());
	debugNode.boom(-length*percent);
	return debugNode.getPosition();
}