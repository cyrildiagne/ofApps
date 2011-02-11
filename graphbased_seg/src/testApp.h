#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include <cstdio>
#include <cstdlib>
#include <image.h>
#include <misc.h>

#include "ofxKinect.h"
#include "ofxSimpleGuiToo.h"

#define SEG_WIDTH 160
#define SEG_HEIGHT 120

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	
private:
	
	ofxKinect kinect;
	
	ofImage segImg;
	ofImage resizeImg;
	
	image<uchar> *input;
	
	float sigma, k;
	int minSize, numCCS;
};

#endif
