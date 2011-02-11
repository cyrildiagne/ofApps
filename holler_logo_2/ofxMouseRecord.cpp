/*
 *  ofxMouseRecord.cpp
 *  holler_logo_2
 *
 *  Created by kikko on 2/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxMouseRecord.h"

ofxMouseRecord::ofxMouseRecord() {
	
	bRecording = false;
	bMouseDown = false;
	bPlaying = false;
}

void ofxMouseRecord::toggleRecording(){
	if (bRecording) {
		stopRecording();
	}
	else {
		startRecording();
	}
}

void ofxMouseRecord::record(string fileName){
	
	bRecording = true;
	
	recordFile = fileName;
	
	xml.addTag("mouse");
	xml.pushTag("mouse", 0);
	
	ofAddListener(ofEvents.update, this, &ofxMouseRecord::updateRecord);
	ofAddListener(ofEvents.mousePressed, this, &ofxMouseRecord::mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &ofxMouseRecord::mouseReleased);
}

void ofxMouseRecord::stopRecording(){
	
	bRecording = false;
	
	xml.popTag();
	
	ofRemoveListener(ofEvents.update, this, &ofxMouseRecord::updateRecord);
	ofRemoveListener(ofEvents.mousePressed, this, &ofxMouseRecord::mousePressed);
	ofRemoveListener(ofEvents.mouseReleased, this, &ofxMouseRecord::mouseReleased);
	
	save();
}

void ofxMouseRecord::play(string fileName){
	
	xml.loadFile(fileName);
	xml.pushTag("mouse", 0);
	
	totalPos = xml.getNumTags("p");
	currPosId = 0;
	
	ofAddListener(ofEvents.update, this, &ofxMouseRecord::updatePlayback);
}

void ofxMouseRecord::updatePlayback(ofEventArgs &args){
	
	string newPosValue = xml.getValue("p", "", currPosId);
	
	if(newPosValue == "") return;
	
	vector<string> infos = ofSplitString(newPosValue, ",");
	
	CGPoint point;
	point.x = ofToInt(infos[0]);
	point.y = ofToInt(infos[1]);
	CGWarpMouseCursorPosition(point);
	
	currPosId++;
	if(currPosId==totalPos) {
		stopPlaying();
	}
}

void ofxMouseRecord::stopPlaying(){
	
	ofRemoveListener(ofEvents.update, this, &ofxMouseRecord::updatePlayback);
	bPlaying = false;
}

void ofxMouseRecord::mousePressed(ofMouseEventArgs &args){
	bMouseDown = true;
}

void ofxMouseRecord::mouseReleased(ofMouseEventArgs &args){
	bMouseDown = false;
}

void ofxMouseRecord::updateRecord(ofEventArgs &args){
	
	xml.addValue("p", ofToString(ofGetAppPtr()->mouseX)+","
					+ ofToString(ofGetAppPtr()->mouseY)+","
					+ (bMouseDown?"1":"0"));
}

void ofxMouseRecord::save(){
	
	xml.saveFile("mouse_record.xml");
}

void ofxMouseRecord::draw(){
	
	if(bPlaying) {
		ofSetColor(0, 0, 255);
		ofTriangle(10, 10, 20, 15, 10, 20);
		return;
	}
	
	if(bRecording){
		ofSetColor(255, 0, 0);
	}else {
		ofSetColor(125, 125, 125);
	}
	
	ofCircle(20, 20, 10);
	
}