#pragma once

#include "ofMain.h"
#include "hollerLogoApp.h"

// internal
#include "Trail.h"

#define LINE_SPACE 8

#define MAX_TRAILS 300

class testApp : public hollerLogoApp{

public:
	
    void setup();
	
    void updateApp();
    void drawApp();
	void resetApp();
	
	void mouseDragged(int x, int y, int button);
	
private:
	
	
	ofImage bg;
	
	vector<Trail> trails;
	
	ofPoint prevMouse;
};
