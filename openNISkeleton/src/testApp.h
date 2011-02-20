#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenNI.h"

#define kNumTestNodes 11

using namespace xn;

class BoneTest : public ofNode {
	
public:
	
	ofQuaternion bindPoseOrientation;
	
	void customDraw() {
		
		ofSetColor(255, 255, 255);
		
		glPushMatrix();
		glTranslatef(0, -40/2, 0);
		glScalef(3, 30, 3);
		glColor3ub(255, 255, 255);
		ofBox(0, 0, 0, 1);
		glPopMatrix();		
	}
};

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	void draw3DScene();
	void drawPointCloud();
	
	void transformNode(int nodeNum, XnSkeletonJoint skelJoint);
	
	ofxOpenNIContext context;
	ofxImageGenerator image;
	ofxDepthGenerator depth;
	ofxUserGenerator user;
		
	BoneTest	testNodes[kNumTestNodes];
	ofLight		light;
	ofCamera	cam;
	
private:
	
	bool bOrbit, bDebug;
	int distance;
};

#endif
