#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	//ofRegisterTouchEvents(this);
	
	ofAddListener(ofEvents.touchDoubleTap, this, &testApp::touchDoubleTap);
    ofAddListener(ofEvents.touchDown, this, &testApp::touchDown);
    ofAddListener(ofEvents.touchMoved, this, &testApp::touchMoved);
    ofAddListener(ofEvents.touchUp, this, &testApp::touchUp);
    
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
    
    stringAR = [ [ ofxString alloc ] init ];
    
    //[[StringOGL sogl] initWithDelegate: this context: aContext frameBuffer: [eaglView defaultFramebuffer] leftHanded: NO];
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
}
