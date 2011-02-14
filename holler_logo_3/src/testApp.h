#pragma once

#include "ofMain.h"
#include "hollerLogoApp.h"

// addons
#include "ofxBox2d.h"

#define LINE_SPACE 3

class testApp : public hollerLogoApp{

public:
    void setup();
	
    void updateApp();
    void drawApp();
	void resetApp();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
		
	b2Vec2 getB2Vec(float x, float y);
	void setupJoint(ofxBox2dJoint &joint, ofxBox2dBaseShape &s1, ofxBox2dBaseShape &s2, float p1x, float p1y, float p2x, float p2y);
	
	void addLettersToBox2D();

private:
	
	ofImage logoWhite;
	ofImage bg;
		
	ofxBox2d box2d;

	vector<ofxBox2dPolygon> lettersPoly;
	vector<ofxBox2dJoint> joints;
	vector<ofxBox2dCircle> circles;
	ofxBox2dRect anchor;
	
	ofPoint initPos;
	ofVec2f getOffset(ofxBox2dPolygon &poly);
	
	vector<ofImage> lettersImg;
	vector<ofVec2f> offsets;
	
	bool bShapeGrabbed;
};
