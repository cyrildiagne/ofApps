#pragma once

#include "ofMain.h"
#include "hollerLogoApp.h"

// addons
#include "ofxTriangle.h"

// internal
#include "Trail.h"

#define LINE_SPACE 3

class testApp : public hollerLogoApp {

public:
	
    void setup();
	
    void updateApp();
    void drawApp();
	void resetApp();
	
	void mouseDragged(int x, int y, int button);
	
private:
	
	ofImage logoWhite;
	ofImage bg;
	
	ofxTriangle triangle;
	
	vector<Trail> trails;
};
