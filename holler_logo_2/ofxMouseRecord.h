/*
 *  ofxMouseRecord.h
 *  holler_logo_2
 *
 *  Created by kikko on 2/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxMouseRecord {
	
public:
	
	ofxMouseRecord();
	
	void toggleRecording();
	void record(string fileName);
	void stopRecording();
	
	void play(string fileName);
	void stopPlaying();
	
	void save();
	
	void draw();
	
	bool bRecording;
	bool bPlaying;
	
private:
	
	ofxXmlSettings xml;
	
	
	// recording
	
	void updateRecord(ofEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	void mouseReleased(ofMouseEventArgs &args);
	
	bool bMouseDown;
	string recordFile;
	
	//playback
	
	void updatePlayback(ofEventArgs &args);
	
	int totalPos;
	int currPosId;
	
};