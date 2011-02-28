/*
 *  UserPointCloud.cpp
 *  skin
 *
 *  Created by kikko on 2/24/11.
 *  Copyright 2011 Cyril Diagne. All rights reserved.
 *
 */

#include "UserPointCloud.h"

UserPointCloud::UserPointCloud() {
	
	verticesBonesId = NULL;
	verticesBindMatrices = NULL;
	finalVerts = NULL;
	
	bBonesInit = false;
}

void UserPointCloud::init(ofxDepthGenerator& depth, ofxImageGenerator& image, ofxUserGenerator& user) {
	
	PointCloud::init(depth, image, user, POINTCLOUD_USER);
}

void UserPointCloud::initBindPose() {
	
	for(int i=0; i<kNumTestNodes; i++) {
		
		// setup a traditional humanoid bind pose
		ofQuaternion q;
		if(i==LEFT_SHOULDER||i==LEFT_ELBOW){
			q.makeRotate(-90.0, ofVec3f(0.0f, 0.0f, 1.0f));
		} else if(i==RIGHT_SHOULDER||i==RIGHT_ELBOW){
			q.makeRotate(90.0, ofVec3f(0.0f, 0.0f, 1.0f));
		} else if(i==NECK){
			q.makeRotate(180.0, ofVec3f(0.0f, 0.0f, 1.0f));
		} else {
			q.makeRotate(0, 0, 0, 1);
		}
		bones[i].bindPose.setRotate(q);
	}
}

void UserPointCloud::initBonesWeights() {
	
	if(verticesBonesId!=NULL) {
		delete[] verticesBonesId;
		delete[] verticesBindMatrices;
		delete[] finalVerts;
	}
	verticesBonesId = new int[numUserPixels];
	verticesBindMatrices = new ofMatrix4x4[numUserPixels];
	finalVerts = new ofVec3f[numUserPixels];
	
	float sqDist, minDist;
	
	for (int i=0; i<numUserPixels; i++) {
		
		minDist = 9999999;
		int minId = -1;
		
		for (int j=0; j<11; j++) {
			
			sqDist = bones[j].getPositionAlong(1).squareDistance(vertices[i]);
			if( sqDist<minDist ) {
				minDist = sqDist;
				minId = j;
			}
		}
		
		verticesBonesId[i] = minId;
		vertices[i] -= bones[minId].getPosition();
		//verticesBindMatrices[i].setPosition( -bones[minId].getPosition() );
	}
	
	vbo.setColorData(colors, numUserPixels, GL_STATIC_DRAW);
}

void UserPointCloud::update() {
	
	if (user->getTrackedUsers().size()>0) {
		
		if(!bBonesInit) {
			
			PointCloud::update();
			initBindPose();
			updateBones();
			initBonesWeights();
			
			bBonesInit = true;
		}
		else {
			
			updateBones();
			Bone * b; ofQuaternion q; float angle; ofVec3f axis;
			for (int i=0; i<numUserPixels; i++) {
				b = &bones[ verticesBonesId[i] ];
				finalVerts[i] = vertices[i];
				
				b->getGlobalOrientation().getRotate(angle, axis);
				finalVerts[i].rotate(angle, axis);
				finalVerts[i] += b->getPosition();
			}
			
			vbo.setVertexData(finalVerts, numUserPixels, GL_DYNAMIC_DRAW);
		}
	}
}

void UserPointCloud::updateBones() {
	
	transformNode(WAIST, XN_SKEL_WAIST);
	transformNode(TORSO, XN_SKEL_TORSO);
	transformNode(NECK, XN_SKEL_NECK);
	transformNode(LEFT_SHOULDER, XN_SKEL_LEFT_SHOULDER);
	transformNode(RIGHT_SHOULDER, XN_SKEL_RIGHT_SHOULDER);
	transformNode(LEFT_ELBOW, XN_SKEL_LEFT_ELBOW);
	transformNode(RIGHT_ELBOW, XN_SKEL_RIGHT_ELBOW);
	transformNode(LEFT_HIP, XN_SKEL_LEFT_HIP);
	transformNode(RIGHT_HIP, XN_SKEL_RIGHT_HIP);
	transformNode(LEFT_KNEE, XN_SKEL_LEFT_KNEE);
	transformNode(RIGHT_KNEE, XN_SKEL_RIGHT_KNEE);
}


//--------------------------------------------------------------
void UserPointCloud::transformNode(int nodeNum, XnSkeletonJoint skelJoint){
	
	// Adapted code from OpenNI Simbad example
	
	XnUserID userId = user->getTrackedUser(0)->id;
	
	// Get the openNI bone info	
	xn::SkeletonCapability pUserSkel = user->getXnUserGenerator().GetSkeletonCap();		
	
	XnSkeletonJointOrientation jointOri;
	pUserSkel.GetSkeletonJointOrientation(userId, skelJoint, jointOri);
	
	XnSkeletonJointPosition jointPos;
	pUserSkel.GetSkeletonJointPosition(userId, skelJoint, jointPos);
	
	if(jointOri.fConfidence > 0 )
	 {
		float * oriM = jointOri.orientation.elements;
		
		ofMatrix4x4 rotMatrix;
		
		// Create a 4x4 rotation matrix (converting row to column-major)
		rotMatrix.set(oriM[0], oriM[3], oriM[6], 0.0f,
					  oriM[1], oriM[4], oriM[7], 0.0f,
					  oriM[2], oriM[5], oriM[8], 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
		
		ofQuaternion q = rotMatrix.getRotate();
		
		bones[nodeNum].setPosition(jointPos.position.X, jointPos.position.Y, jointPos.position.Z);
		
		// apply skeleton pose relatively to the bone bind pose
		// /!\ WARNING the order of the quat' multiplication does mater!!
		bones[nodeNum].setOrientation(bones[nodeNum].bindPose.getRotate()*q);
	 }
}

void UserPointCloud::draw() {
	
	if(!allocated) return;
	
	for(int i=0; i<kNumTestNodes; i++) {
		bones[i].draw();
	}
	
	vbo.draw(GL_POINTS, 0, numUserPixels);
}