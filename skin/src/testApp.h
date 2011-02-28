#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenNI.h"

#include "UserPointCloud.h"
#include "Bone.h"
#include "Global.h"

#define PLAYBACK

using namespace xn;

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
			
	ofxOpenNIContext	context;
	ofxImageGenerator	image;
	ofxDepthGenerator	depth;
	ofxUserGenerator	user;
	
#ifndef PLAYBACK
	ofxOpenNIRecorder	recorder;
#endif
	
	ofCamera	camera;
	
	UserPointCloud	userPointCloud;
	PointCloud		bgPointCloud;
	PointCloud		scenePointCloud;
	
private:
	
	bool bOrbit, bDebug, bRecord, bRecordBackground;
	int distance;
	
	float camFOV;
};

#endif
