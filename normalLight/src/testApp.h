#pragma once

#include "ofMain.h"
#include "LightenImage.h"

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
	
	LightenImage buddah;
	
	ofEasyCam camera;
	ofLight light;
};
