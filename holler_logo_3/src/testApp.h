#pragma once

#include "ofMain.h"

// addons
#include "ofxOpenCv.h"
#include "ofxBox2d.h"

// internal
#include "Assets.h"
#include "Border.h"

#define LINE_SPACE 3

#define LETTER_O_OUTLINE 0
#define LETTER_H		 1
#define LETTER_E_OUTLINE 2
#define LETTER_L		 3
#define LETTER_R		 4
#define LETTER_O_INLINE  5
#define LETTER_L2		 6
#define LETTER_E_INLINE	 7
#define LETTER_O_ACCENT	 8


class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
	void keyPressed(int key);
	
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	
	void makeComplexBody();
	
	b2Vec2 getB2Vec(float x, float y);
	void setupJoint(ofxBox2dJoint &joint, ofxBox2dBaseShape &s1, ofxBox2dBaseShape &s2, float p1x, float p1y, float p2x, float p2y);
	
	void loadContours();
	void addLettersToBox2D();

private:
	
	ofImage logo;
	ofImage logoWhite;
	ofImage bg;
	
	Border screenBounds;
	void setScreenBounds();
	
	vector<Border> letters;
		
	ofxBox2d		box2d;

	vector<ofxBox2dPolygon> lettersPoly;
	vector<ofxBox2dJoint> joints;
	vector<ofxBox2dCircle> circles;
	ofxBox2dRect anchor;
	
	ofPoint initPos;
	ofVec2f getOffset(ofxBox2dPolygon &poly);
	
	vector<ofImage> lettersImg;
	vector<ofVec2f> offsets;
	
	bool bDebug;
};
