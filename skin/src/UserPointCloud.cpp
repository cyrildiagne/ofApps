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
	bBonesInit = false;
}

void UserPointCloud::init(ofxONI &oni) {
	
	shaderFile = "shaders/userpointcloud";
    
	PointCloud::init(oni, POINTCLOUD_USER);
}

void UserPointCloud::update() {
	
    if (user->getTrackedUsers().size()>0) {
		if(!bBonesInit) initBones();
		else updateBones();
	}
}

void UserPointCloud::initBones() {
	
	// First retrieve the points of the user
	PointCloud::update();
	
	ofVec3f zAxis = ofVec3f(0.0f, 0.0f, 1.0f);
	
	ofQuaternion qDefault	= ofQuaternion( 0	 , zAxis);
	ofQuaternion qLeft		= ofQuaternion(-90.0 , zAxis);
	ofQuaternion qRight		= ofQuaternion( 90.0 , zAxis);
	ofQuaternion qNeck		= ofQuaternion( 180.0, zAxis);
	
	bones[TORSO]		 .init( XN_SKEL_TORSO,			qDefault);
	bones[WAIST]		 .init( XN_SKEL_WAIST,			qDefault);
	bones[LEFT_SHOULDER] .init( XN_SKEL_LEFT_SHOULDER,	qLeft);
	bones[LEFT_ELBOW]	 .init( XN_SKEL_LEFT_ELBOW,		qLeft);
	bones[RIGHT_SHOULDER].init( XN_SKEL_RIGHT_SHOULDER,	qRight);
	bones[RIGHT_ELBOW]	 .init( XN_SKEL_RIGHT_ELBOW,	qRight);
	bones[LEFT_HIP]		 .init( XN_SKEL_LEFT_HIP,		qDefault);
	bones[RIGHT_HIP]	 .init( XN_SKEL_RIGHT_HIP,		qDefault);
	bones[LEFT_KNEE]	 .init( XN_SKEL_LEFT_KNEE,		qDefault);
	bones[RIGHT_KNEE]	 .init( XN_SKEL_RIGHT_KNEE,		qDefault);
	bones[NECK]			 .init( XN_SKEL_NECK,			qNeck);
	
	// Update the bones
	updateBones();
	
	for(int i=0; i<kNumTestNodes; i++) bones[i].updateCalibPose();
	
	setBonesLengths();
	
	initVerticesWeights();
	
	bBonesInit = true;
}


void UserPointCloud::setBonesLengths() {
	
	setBoneLength(NECK,			 XN_SKEL_TORSO);
	setBoneLength(LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW);
	setBoneLength(LEFT_ELBOW,	 XN_SKEL_LEFT_HAND);
	setBoneLength(RIGHT_SHOULDER,XN_SKEL_RIGHT_ELBOW);
	setBoneLength(RIGHT_ELBOW,	 XN_SKEL_RIGHT_HAND);	
	setBoneLength(LEFT_HIP,		 XN_SKEL_LEFT_KNEE);
	setBoneLength(LEFT_KNEE,	 XN_SKEL_LEFT_FOOT);
	setBoneLength(RIGHT_HIP,	 XN_SKEL_RIGHT_KNEE);
	setBoneLength(RIGHT_KNEE,	 XN_SKEL_RIGHT_FOOT);
}

void UserPointCloud::initVerticesWeights() {
	
	verticesBonesId = new float[numUserPixels];
	
	float sqDist, minDist;
	
	for (int i=0; i<numUserPixels; i++) {
		
		minDist = 9999999;
		int minId = -1;
		
		for (int j=0; j<11; j++) {
			
			sqDist = bones[j].getPositionAlong(0.5).squareDistance(getVertex(i));
			if( sqDist<minDist ) {
				minDist = sqDist;
				minId = j;
			}
		}
		
		verticesBonesId[i] = minId;
		//colors[i] = bones[minId].debugColor;
	}
	
	//vbo.setColorData(colors, numUserPixels, GL_STATIC_DRAW);
}

void UserPointCloud::setBoneLength(int boneId, XnSkeletonJoint endBone) {
	
	XnSkeletonJointPosition endPos;
	user->getXnUserGenerator().GetSkeletonCap().GetSkeletonJointPosition(user->getTrackedUser(0)->id, endBone, endPos);
	
	ofPoint dest(endPos.position.X, endPos.position.Y, endPos.position.Z);
	ofPoint origin(bones[boneId].getPosition());
	
	bones[boneId].length = origin.distance(dest);
}

void UserPointCloud::updateBones() {
	
	for (int i=0; i<kNumTestNodes; i++) {
		
		XnUserID userId = user->getTrackedUser(0)->id;
		
		// Get the openNI bone info	
		xn::SkeletonCapability pUserSkel = user->getXnUserGenerator().GetSkeletonCap();		
		
		XnSkeletonJointOrientation jointOri;
		pUserSkel.GetSkeletonJointOrientation(userId, bones[i].xnJointId, jointOri);
		
		XnSkeletonJointPosition jointPos;
		pUserSkel.GetSkeletonJointPosition(userId, bones[i].xnJointId, jointPos);
		
		if( jointOri.fConfidence > 0 )
		 {
			float * oriM = jointOri.orientation.elements;
			
			ofMatrix4x4 rotMatrix;
			
			// Create a 4x4 rotation matrix (converting row to column-major)
			rotMatrix.set(oriM[0], oriM[3], oriM[6], 0.0f,
						  oriM[1], oriM[4], oriM[7], 0.0f,
						  oriM[2], oriM[5], oriM[8], 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f);
			
			ofQuaternion q = rotMatrix.getRotate();
			
			bones[i].setPosition(jointPos.position.X, jointPos.position.Y, jointPos.position.Z);
			
			// apply skeleton pose relatively to the bone bind pose
			// /!\ WARNING the order of the quat' multiplication does mater!!
			bones[i].setOrientation(bones[i].bindPose.getRotate()*q);
		 }
	}
}

void UserPointCloud::drawBones() {
    
    for(int i=0; i<kNumTestNodes; i++) {
        bones[i].draw();
    }
}

void UserPointCloud::draw() {
	
	shader.begin();
    
	// we load the bone transforms in the constant table
	for (int i=0; i<kNumTestNodes; ++i)
	{
	   string boneMatrixUniformName = "boneMatrices["+ofToString(i)+"]";
	   GLint currentMatrix = glGetUniformLocation(shader.getProgram(), boneMatrixUniformName.c_str());
        shader.setUniform4fv(boneMatrixUniformName.c_str(), (bones[i].calibPose * bones[i].getGlobalTransformMatrix()).getPtr()), 16);
        //glUniformMatrix4fv(currentMatrix, 1, GL_FALSE, (bones[i].calibPose * bones[i].getGlobalTransformMatrix()).getPtr());
	}
	
	int bIdAttLoc = shader.getAttributeLocation("boneId");
	glEnableVertexAttribArray(bIdAttLoc);
	glVertexAttribPointer(bIdAttLoc, 1, GL_FLOAT, false, 0, verticesBonesId);
	glBindAttribLocation(shader.getProgram(), bIdAttLoc, "boneId");
	
	ofVboMesh::draw(OF_MESH_POINTS);
    
	shader.end();
	glDisableVertexAttribArray(bIdAttLoc);
}