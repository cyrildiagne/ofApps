/*
 *  UserPointCloud.h
 *  skin
 *
 *  Created by kikko on 2/24/11.
 *  Copyright 2011 Cyril Diagne. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "Bone.h"
#include "Global.h"
#include "PointCloud.h"

using namespace xn;

class UserPointCloud : public PointCloud {
	
public:
	
	UserPointCloud();
	
	void init(ofxDepthGenerator& depth, ofxImageGenerator& image, ofxUserGenerator& user);
	
	void update();
	
	void draw(bool bDrawBones);
	
	bool bBonesInit;
	
private:
	
	void initBones();
	void initVerticesWeights();
	
	void setBonesLengths();
	void setBoneLength(int boneId, XnSkeletonJoint endBone);
	
	void updateBones();
	
	float * verticesBonesId;
	
	Bone	bones[kNumTestNodes];
	
	int numBones;
	
};