#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	
	//context.setupUsingRecording("SkeletonRec.oni");
	context.setupUsingXMLFile();
	depth.setup(&context);
	user.setup(&context, &depth);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofDisableArbTex();
	ofSetVerticalSync(true);
	//some model / light stuff
    glShadeModel(GL_SMOOTH);
	
	astroboy.load();
	astroboy.setPosition(300, 300, 500);
	initArmature();
	
	cam.setFov(60);
	cam.setPosition(250, 250, -400);
	cam.rotate(180, 0.0, 1.0, 0.0);
}

//--------------------------------------------------------------
void testApp::initArmature(){
	
	BoneNode* torso		= new BoneNode( astroboy.getBoneNode("root"), XN_SKEL_TORSO);
	BoneNode* neck		= new BoneNode( astroboy.getBoneNode("neck"), XN_SKEL_NECK, torso);
	BoneNode* hips		= new BoneNode( astroboy.getBoneNode("hips"), XN_SKEL_WAIST);
	BoneNode* shoulderL = new BoneNode( astroboy.getBoneNode("L_shoulder"), XN_SKEL_LEFT_SHOULDER, torso);
	BoneNode* shoulderR = new BoneNode( astroboy.getBoneNode("R_shoulder"), XN_SKEL_RIGHT_SHOULDER, torso);
	BoneNode* elbowL	= new BoneNode( astroboy.getBoneNode("L_elbow"), XN_SKEL_LEFT_ELBOW, shoulderL);
	BoneNode* elbowR	= new BoneNode( astroboy.getBoneNode("R_elbow"), XN_SKEL_RIGHT_ELBOW, shoulderR);
	BoneNode* hipL		= new BoneNode( astroboy.getBoneNode("L_hip"), XN_SKEL_LEFT_HIP, hips);
	BoneNode* hipR		= new BoneNode( astroboy.getBoneNode("R_hip"), XN_SKEL_RIGHT_HIP, hips);
	BoneNode* kneeL		= new BoneNode( astroboy.getBoneNode("L_knee"), XN_SKEL_LEFT_KNEE, hipL);
	BoneNode* kneeR		= new BoneNode( astroboy.getBoneNode("R_knee"),	XN_SKEL_RIGHT_KNEE, hipR);
	
	// setup humanoid bind pose
	//torso->bindPoseOrientation.makeRotate(90.0, ofVec3f(1.0f, 0.0f, 0.0f));
	shoulderL->bindPoseOrientation.makeRotate(-90.0, ofVec3f(0.0f, 0.0f, 1.0f));
	elbowL->bindPoseOrientation.makeRotate(-90.0, ofVec3f(0.0f, 0.0f, 1.0f));
	shoulderR->bindPoseOrientation.makeRotate(90.0, ofVec3f(0.0f, 0.0f, 1.0f));
	elbowR->bindPoseOrientation.makeRotate(90.0, ofVec3f(0.0f, 0.0f, 1.0f));
	neck->bindPoseOrientation.makeRotate(180.0, ofVec3f(0.0f, 0.0f, 1.0f));
	
	bones.push_back(torso);
	/*bones.push_back(neck);
	bones.push_back(shoulderL);
	bones.push_back(shoulderR);
	bones.push_back(elbowL);
	bones.push_back(elbowR);
	bones.push_back(hips);
	bones.push_back(hipL);
	bones.push_back(hipR);
	bones.push_back(kneeL);
	bones.push_back(kneeR);*/
}

//--------------------------------------------------------------
void testApp::update(){
	
	context.update();
	user.update();
	
	if(user.getTrackedUsers().size()==0) return;
	
	// Adapted code from OpenNI Simbad example
	XnUserID userId = user.getTrackedUser(0)->id;
	
	// Get the openNI bone info
	SkeletonCapability pUserSkel = user.getXnUserGenerator().GetSkeletonCap();
	
	XnSkeletonJointOrientation jointOri;
	XnSkeletonJointPosition jointPos;
	
	for (int i=0; i<bones.size(); i++) {
		
		pUserSkel.GetSkeletonJointOrientation(userId, bones[i]->joint, jointOri);
		pUserSkel.GetSkeletonJointPosition(userId, bones[i]->joint, jointPos);
		
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
			
			bones[i]->setGlobalPosition(jointPos.position.X, jointPos.position.Y, jointPos.position.Z);
			
			// apply skeleton pose relatively to the bone bind pose
			// /!\ WARNING the order of the quat' multiplication does mater!!
			bones[i]->setGlobalOrientation(bones[i]->bindPoseOrientation*q);
			
			// get relative matrix
			ofVec4f lMat[4] = bones[i]->getLocalTransformMatrix()._mat;
			
			// create Assimp Matrix
			aiMatrix4x4 trafo(lMat[0][0], -lMat[0][1], lMat[0][2], lMat[0][3],
							  -lMat[1][0], lMat[1][1], -lMat[1][2], lMat[1][3],
							  lMat[2][0], -lMat[2][1], lMat[2][2], lMat[2][3],
							  lMat[3][0], lMat[3][1], lMat[3][2], lMat[3][3]);
			
			// find the corresponding node by again looking recursively through the node hierarchy for the same name
			bones[i]->node->mTransformation = trafo;
		 }
	}
	
	astroboy.update();
}

//--------------------------------------------------------------
void testApp::transformBone(string boneName, XnSkeletonJoint skelJoint){
	
	// Adapted code from OpenNI Simbad example
	
	XnUserID userId = user.getTrackedUser(0)->id;
	
	// Get the openNI bone info
	SkeletonCapability pUserSkel = user.getXnUserGenerator().GetSkeletonCap();		
	
	
	XnSkeletonJointOrientation jointOri;
	pUserSkel.GetSkeletonJointOrientation(userId, skelJoint, jointOri);
	
	XnSkeletonJointPosition jointPos;
	pUserSkel.GetSkeletonJointPosition(userId, skelJoint, jointPos);
	
	
	if(jointOri.fConfidence > 0 )
	 {
		float * oriM = jointOri.orientation.elements;
		
		// find the corresponding node by again looking recursively through the node hierarchy for the same name
		aiNode* node = astroboy.getBoneNode(boneName);
		
		aiVector3D scaling;
		aiQuaternion rotation;
		aiVector3D position;
		node->mTransformation.Decompose(scaling, rotation, position);
		
		aiMatrix4x4 trafo(oriM[0], oriM[3], oriM[6], 0.0f,
						  oriM[1], oriM[4], oriM[7], 0.0f,
						  oriM[2], oriM[5], oriM[8], 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f);
		trafo.a4 = position.x;
		trafo.b4 = position.y;
		trafo.c4 = position.z;
		
		//node->
		
		/*
		ofMatrix4x4 rotMatrix;
		
		// Create a 4x4 rotation matrix (converting row to column-major)
		rotMatrix.set(oriM[0], oriM[3], oriM[6], 0.0f,
					  oriM[1], oriM[4], oriM[7], 0.0f,
					  oriM[2], oriM[5], oriM[8], 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f);
		
		ofQuaternion q = rotMatrix.getRotate();
		
		testNodes[nodeNum].setPosition(jointPos.position.X, jointPos.position.Y, jointPos.position.Z);
		
		// apply skeleton pose relatively to the bone bind pose
		// /!\ WARNING the order of the quat' multiplication does mater!!
		testNodes[nodeNum].setOrientation(testNodes[nodeNum].bindPoseOrientation*q);
		 */
	 }
}

//--------------------------------------------------------------
void testApp::draw(){
		
	ofSetColor(255, 255, 255);
	depth.draw(0,0,640,480);
	user.draw();
	
	ofxTrackedUser* tracked = user.getTrackedUser(0);
	if(tracked != NULL) {
		tracked->debugDraw();
	}
	
	// setup scene Rendering
	
	//ofEnableLighting();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);
		
	cam.begin();
	
	draw3DScene();
	
	cam.end();
		
	ofDisableLighting();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHT0);
}

void testApp::draw3DScene(){
	
	
	//light.draw();
	
	ofDrawAxis(100);
	
	for(int i=0; i<bones.size(); i++) {
		bones[i]->draw();
	}
	
	astroboy.draw();
	
}

