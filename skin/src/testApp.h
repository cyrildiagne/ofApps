#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenNI.h"

//#include "ofxQtVideoSaver.h"

#include "UserPointCloud.h"
#include "Bone.h"
#include "Global.h"

#define PLAYBACK
//#define EXPORT_VIDEO

using namespace xn;

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
    
    ofxONI  oni;
	
#ifndef PLAYBACK
	ofxOpenNIRecorder	recorder;
#endif
	
	ofCamera	camera;
	
	UserPointCloud	userPointCloud;
	PointCloud		bgPointCloud;
	PointCloud		scenePointCloud;
	
private:
	
	bool bOrbit, bDrawSkinnedUserPointCloud, bDebug, bRecord, bRecordBackground;
	int distance;

#ifdef EXPORT_VIDEO
	ofxQtVideoSaver videoSaver;
#endif
    
	ofImage screen;
	
	float camFOV;
};

#endif
