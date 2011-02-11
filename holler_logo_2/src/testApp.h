#pragma once

#include "ofMain.h"

// addons
#include "ofxOpenCv.h"
#include "ofxMouseRecord.h"
//#include "ofxTriangle.h"

// internal
#include "Assets.h"
#include "Border.h"
#include "Trail.h"

#define LINE_SPACE 3

#define LETTER_O_OUTLINE 0
#define LETTER_H		 1
#define LETTER_E_OUTLINE 2
#define LETTER_L		 3
#define LETTER_R		 4
#define LETTER_O_INLINE  5
#define LETTER_L2		 6
#define LETTER_E_INLINE	 7

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
	void keyPressed(int key);
	
	void mouseDragged(int x, int y, int button);
	
    void gotMessage(ofMessage msg);
	
	void loadContours();
	void initLines();

private:
	
	ofImage logo;
	ofImage logoWhite;
	ofImage bg;
	
	ofFbo fbo;
	ofxMouseRecord mouseRecord;
	
	//ofxTriangle triangle;
	
	Border screenBounds;
	void setScreenBounds();
	
	vector<Border> letters;
	vector<Trail> trails;
	
	bool bDebug;
};
