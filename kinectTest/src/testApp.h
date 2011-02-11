
 #ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxKinect.h"

#include "Cloth.h"
#include "KinectNormal.h"

class testApp : public ofBaseApp
{

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	
	Cloth cloth;
	
	ofxKinect kinect;
	//KinectNormal kinectNormal;

	ofImage depthImage;
	int nearThreshold, farThreshold;
	
	ofImage rgbaImg;
	unsigned char * rgbaPixels;
	
	bool bSetup;
};

#endif