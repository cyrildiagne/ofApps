#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenNI.h"

#define kNumTestNodes 11

using namespace xn;

class BoneTest : public ofMeshNode {
	
public:
	
	ofQuaternion bindPoseOrientation;
	
	void customDraw() {
		
		ofSetColor(255, 255, 255);
		
		glPushMatrix();
		glTranslatef(0, -40/2, 0);
		glScalef(5, 40, 5);
		ofBox(0, 0, 0, 1);
		glPopMatrix();
		
		ofDrawAxis(20);		
	}
};

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	void draw3DScene();
	
	void transformNode(int nodeNum, XnSkeletonJoint skelJoint);
	
	ofxOpenNIContext context;
	ofxDepthGenerator depth;
	ofxUserGenerator user;
	
	BoneTest	testNodes[kNumTestNodes];
	ofLight		light;
	ofCamera	cam;
};

#endif
