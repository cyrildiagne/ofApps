#pragma once

#include "ofMain.h"

// addons
#include "ofxOpenCv.h"

// internal
#include "Assets.h"
#include "Border.h"
#include "Trail.h"

#define LINE_SPACE 8

#define LETTER_O_OUTLINE 0
#define LETTER_H		 1
#define LETTER_E_OUTLINE 2
#define LETTER_L		 3
#define LETTER_R		 4
#define LETTER_O_INLINE  5
#define LETTER_L2		 6
#define LETTER_E_INLINE	 7
#define LETTER_O_ACCENT	 8

#define MAX_TRAILS 300

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
	
	ofFbo fbo;
	
	ofImage logo;
	ofImage bg;
	
	Border screenBounds;
	void setScreenBounds();
	
	vector<Border> letters;
	vector<Trail> trails;
	
	ofPoint prevMouse;
	
	bool bDebug;
};
