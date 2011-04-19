#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	 
	buddah.setup();
	
	camera.setTarget(ofVec3f(0, 0, 0));
	camera.cacheMatrices = true;
}

//--------------------------------------------------------------
void testApp::update(){
	
	light.setPosition( ofGetMouseX() - ofGetWidth() * 0.5,
					  ofGetMouseY() - ofGetHeight() * 0.5,
					  100.0);
}

//--------------------------------------------------------------
void testApp::draw(){
    
    camera.begin();
    
	ofDrawAxis(200);
	
    buddah.draw(camera, light);
	
	ofSetColor(255, 255, 255);
	ofSphere(light.getPosition(), 20);
	
	camera.end();
}