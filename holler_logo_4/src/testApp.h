#pragma once

#include "ofMain.h"
#include "hollerLogoApp.h"

// internal
#include "Assets.h"
#include "Border.h"
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
	
	vector<int> numTrailsInside;
	vector<Trail> trails;
	
	float linePoints[3][3];
	float colors[3][4];
	
	bool bDebug;
};
