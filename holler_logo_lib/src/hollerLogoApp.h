/*
 *  hollerLogoApp.h
 *  holler_logo_2
 *
 *  Created by Cyril Diagne on 14/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofEvents.h"

#include "Assets.h"
#include "Border.h"

#include "ofxMouseRecord.h"
#include "ofxOpenCv.h"
#include "ofxQtVideoSaver.h"

#define LETTER_O_OUTLINE 0
#define LETTER_H		 1
#define LETTER_E_OUTLINE 2
#define LETTER_L		 3
#define LETTER_R		 4
#define LETTER_O_INLINE  5
#define LETTER_L2		 6
#define LETTER_E_INLINE	 7
#define LETTER_O_ACCENT	 8

class hollerLogoApp : public ofBaseApp {
	
public:
	
    void setup();
	void update(ofEventArgs &args);
	void draw(ofEventArgs &args);
	
	void keyPressed(ofKeyEventArgs &args);
	
	virtual void updateApp(){};
	virtual void drawApp(){};
	virtual void resetApp(){};
	
protected:
	
	ofFbo fbo;
	
	Border screenBounds;
	void setScreenBounds();
	
	// LETTERS
	
	void loadContours();
	
	vector<Border> letters;
	
	bool bDebug;
		
	// RECORDING
	
	ofxMouseRecord mouseRecord;
	ofxQtVideoSaver	saver;
	
	void record(string fileName);
	void endRecording();
	
	ofImage screen;
	
	bool bRecording;
	
};
