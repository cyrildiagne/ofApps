/*
 *  hollerLogoApp.cpp
 *  holler_logo_2
 *
 *  Created by Cyril Diagne on 14/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "hollerLogoApp.h"

void hollerLogoApp::setup(){
	
	ofSetFrameRate(60);
	
	bRecording = false;
	bDebug = false;
	
	ofAddListener(ofEvents.update, this, &hollerLogoApp::update);
	ofAddListener(ofEvents.draw, this, &hollerLogoApp::draw);
	
	ofAddListener(ofEvents.keyPressed, this, &hollerLogoApp::keyPressed);
	
	fbo.setup(ofGetWidth(), ofGetHeight(), GL_RGBA, 2);
	
	setScreenBounds();
	
    loadContours();
};

void hollerLogoApp::setScreenBounds(){
	
	vector<ofPoint> pts;
	pts.push_back(ofPoint(0, 0));
	pts.push_back(ofPoint(ofGetWidth(), 0));
	pts.push_back(ofPoint(ofGetWidth(), ofGetHeight()));
	pts.push_back(ofPoint(0, ofGetHeight()));
	
	screenBounds.set(pts);
}

void hollerLogoApp::loadContours(){
	
	ofImage logo;
	logo.loadImage("logo.png");
	logo.setImageType(OF_IMAGE_GRAYSCALE);
	
	ofxCvGrayscaleImage logoCv;
	logoCv.allocate(logo.width, logo.height);
	logoCv.setFromPixels(logo.getPixels(), logo.width, logo.height);
	
	ofxCvContourFinder contourFinder;
	contourFinder.findContours(logoCv, 20*20, 300*300, 10, true, false);
	
	for(int i=0; i<contourFinder.blobs.size(); i++){
		letters.push_back( Border(contourFinder.blobs[i].pts, 10.f) );
	}
	
	letters[LETTER_O_OUTLINE].addHole(&letters[LETTER_O_INLINE]);
	letters[LETTER_E_OUTLINE].addHole(&letters[LETTER_E_INLINE]);
}

void hollerLogoApp::update(ofEventArgs &args){
	
	updateApp();
	
	if(bRecording && !mouseRecord.bPlaying) {
		
		endRecording();
		cout << "recording stoped" << endl;
	}
};

void hollerLogoApp::draw(ofEventArgs &args) {
	
	fbo.begin();
		drawApp();
	fbo.end();
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
	//fbo.draw(0, ofGetHeight(), fbo.getWidth(), -fbo.getHeight());
	fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	
	if(bRecording){
		
		ofFill();
		ofSetColor(125, 125, 125, 255);
		ofCircle(mouseX, mouseY, 11);
		ofSetColor(255, 255, 255, 255);
		ofCircle(mouseX, mouseY, 10);
		ofSetColor(125, 125, 125, 255);
		ofCircle(mouseX, mouseY, 2);
		
		screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		saver.addFrame(screen.getPixels(), 1.0f / 60.0f); 
	}
	
	mouseRecord.draw();
	
	ofSetColor(0xff, 0xff, 0xff);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), ofGetWidth()-40, ofGetHeight()-10);
}

void hollerLogoApp::record(string fileName) {
	
	screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
	
	saver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_LOSSLESS);
	saver.setup(ofGetWidth(),ofGetHeight(), fileName);
}

void hollerLogoApp::endRecording() {
	
	saver.finishMovie();
	
	bRecording = false;
}

void hollerLogoApp::keyPressed(ofKeyEventArgs &args){
	
	switch (args.key) {
		case 'd':
			bDebug = !bDebug;
		case 'r':
			resetApp();
			break;
		case 'm':
			if (mouseRecord.bRecording) {
				mouseRecord.stopRecording();
			}
			else {
				resetApp();
				mouseRecord.record("mouse_record.xml");
			}
			break;
		case 'p':
			if (mouseRecord.bPlaying) {
				mouseRecord.stopPlaying();
			}
			else {
				resetApp();
				mouseRecord.play("mouse_record.xml");
			}
			
			break;
		case 'v':
			resetApp();
			record("output.mov");
			mouseRecord.play("mouse_record.xml");
			bRecording = true;
			break;
			
		default:
			break;
	}
}