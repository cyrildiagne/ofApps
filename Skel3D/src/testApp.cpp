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
	
	cam.setFov(60);
	cam.setPosition(250, 250, -400);
	cam.rotate(180, 0.0, 1.0, 0.0);
	
	for(int i=0; i<kNumTestNodes; i++) {
		
		testNodes[i].setPosition(ofRandomf()*10-5, ofRandomf()*10-5, ofRandomf()*10-5);
		testNodes[i].setScale(10);
		
		// setup a traditional humanoid bind pose
		
		// LEFT ARM
		if(i==2||i==4){
			testNodes[i].bindPoseOrientation.makeRotate(-90.0, ofVec3f(0.0f, 0.0f, 1.0f));
		// RIGHT ARM
		} else if(i==3||i==5){
			testNodes[i].bindPoseOrientation.makeRotate(90.0, ofVec3f(0.0f, 0.0f, 1.0f));
		// NECK
		} else if(i==10){
			testNodes[i].bindPoseOrientation.makeRotate(180.0, ofVec3f(0.0f, 0.0f, 1.0f));
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
	
	context.update();
	user.update();
	
	if(user.getTrackedUsers().size()>0) {
		
		transformNode(0, XN_SKEL_TORSO);
		transformNode(1, XN_SKEL_WAIST);
		transformNode(2, XN_SKEL_LEFT_SHOULDER);
		transformNode(3, XN_SKEL_RIGHT_SHOULDER);
		transformNode(4, XN_SKEL_LEFT_ELBOW);
		transformNode(5, XN_SKEL_RIGHT_ELBOW);
		transformNode(6, XN_SKEL_LEFT_HIP);
		transformNode(7, XN_SKEL_RIGHT_HIP);
		transformNode(8, XN_SKEL_LEFT_KNEE);
		transformNode(9, XN_SKEL_RIGHT_KNEE);
		transformNode(10, XN_SKEL_NECK);
	}
}

//--------------------------------------------------------------
void testApp::transformNode(int nodeNum, XnSkeletonJoint skelJoint){
	
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
	
	glEnable(GL_LIGHTING);
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
	glDisable(GL_LIGHTING);
}

void testApp::draw3DScene(){
	
	
	//light.draw();
	
	ofDrawAxis(100);
	
	for(int i=0; i<kNumTestNodes; i++) {
		testNodes[i].draw();
	}

}

