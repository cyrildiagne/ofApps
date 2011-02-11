#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenNI.h"
#include "ofxAssimpModelLoader.h"

#include "Model.h"
#include "BoneNode.h"

using namespace xn;

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	void draw3DScene();
	void initArmature();
	
	void transformBone(string boneName, XnSkeletonJoint skelJoint);
	
	ofxOpenNIContext context;
	ofxDepthGenerator depth;
	ofxUserGenerator user;
	
	Model astroboy;
	
	vector<BoneNode*> bones;
	
	ofLight		light;
	ofCamera	cam;
};

#endif
