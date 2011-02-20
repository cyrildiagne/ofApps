/*
 *  Model.h
 *  Skel3D
 *
 *  Created by kikko on 1/21/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BONE_NODE
#define _BONE_NODE

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxOpenNI.h"

class BoneNode : public ofNode {
	
public:
	
	ofQuaternion bindPoseOrientation;
	XnSkeletonJoint joint;
	aiNode* node;
	BoneNode* parent;
	
	BoneNode(aiNode* node, XnSkeletonJoint joint, BoneNode* parent=NULL){
		
		this->joint = joint;
		this->node = node;
		this->parent = parent;
		
		aiVector3D scaling;
		aiQuaternion rotation;
		aiVector3D position;
		node->mTransformation.Decompose(scaling, rotation, position);
		
		//setPosition(ofRandom(200), ofRandom(200), 1000+ofRandom(1000));
		
		setParent(*parent);
		
		if(parent!=NULL){
			//setPosition(200+parent->position.x+position.x, 200+parent->position.y+position.y, parent->position.z+position.z);
		}
		else {
			//setPosition(200+position.x, 200+position.y, position.z);
		}		
	}
	
	void customDraw() {
		
		ofSetColor(255, 255, 255);
		
		glPushMatrix();
		glTranslatef(-25, -200, -25);
		glScalef(50, 400, 50);
		ofBox(0, 0, 0, 1);
		glPopMatrix();
		
		ofDrawAxis(200);		
	}
};

#endif