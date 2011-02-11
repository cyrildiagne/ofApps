#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground( ofColor(0, 0, 0) );
	//ofSetBackgroundAuto(false);
	ofSetFrameRate(60);
	
	logo.loadImage("logo.png");
	logo.setImageType(OF_IMAGE_GRAYSCALE);
	
	bg.loadImage("bg_dark.png");
	
	bDebug = false;
	
	fbo.setup(ofGetWidth(), ofGetHeight(), GL_RGBA, 1);
	
	setScreenBounds();
	
    loadContours();
	
	initLines();
}

void testApp::setScreenBounds(){
	
	vector<ofPoint> pts;
	pts.push_back(ofPoint(0, 0));
	pts.push_back(ofPoint(ofGetWidth(), 0));
	pts.push_back(ofPoint(ofGetWidth(), ofGetHeight()));
	pts.push_back(ofPoint(0, ofGetHeight()));
	
	screenBounds.set(pts);
}

void testApp::loadContours(){
	
	ofxCvGrayscaleImage logoCv;
	logoCv.allocate(logo.width, logo.height);
	logoCv.setFromPixels(logo.getPixels(), logo.width, logo.height);
	
	ofxCvContourFinder contourFinder;
	contourFinder.findContours(logoCv, 20*20, 300*300, 10, true, true);
	
	for(int i=0; i<contourFinder.blobs.size(); i++){
		
		letters.push_back( Border(contourFinder.blobs[i].pts, 10.f) );
	}
	
	letters[LETTER_O_OUTLINE].addHole(&letters[LETTER_O_INLINE]);
	letters[LETTER_E_OUTLINE].addHole(&letters[LETTER_E_INLINE]);
}

void testApp::initLines(){
	
	int numColumns = logo.width / LINE_SPACE;
	int numRows = logo.height / LINE_SPACE;
	
	float margin = 10.f;
	/*
	for(int i=0; i<numColumns; i++){
		trails.push_back( Trail( ofPoint(i*LINE_SPACE, margin), ofPoint(1, 1).getNormalized() ) );
		trails.push_back( Trail( ofPoint((i+0.5)*LINE_SPACE, logo.height-margin), ofPoint(-1, -1).getNormalized() ) );
	}
	for(int i=0; i<numRows; i++){
		trails.push_back( Trail( ofPoint(margin, i*LINE_SPACE), ofPoint(1, 1).getNormalized() ) );
		trails.push_back( Trail( ofPoint(logo.width-margin, (i+0.5)*LINE_SPACE), ofPoint(-1, -1).getNormalized() ) );
	}*/
}


//--------------------------------------------------------------
void testApp::update(){
	
	ofPoint * pos;
	Trail *trail;
	
	bool bInsideLetter;
	
	for(int i=0; i<trails.size(); i++){
		
		trail = &trails[i];
		pos = &trail->position;
		
		bInsideLetter = false;
		
		// look if it has found a letter now
		for(int j=0; j<letters.size(); j++){
			if( j != LETTER_O_INLINE
				&& j != LETTER_E_INLINE
				&& ofInsidePoly( *pos, letters[j].points ) ) {
				
				trail->letterId = j;
				bInsideLetter = true;
				break;
			}
		}
		
		if(bInsideLetter){
			//trail->color.set(229, 69, 65, 0xff);
			trail->update(letters[trail->letterId]);
		}
		else {
			//trail->color.set(182, 49, 51, 0xAA);
			trail->update(screenBounds);
		}

		
		/*
		// If trail isn't in a letter yet
		if(trail->letterId == -1) {
			
			trail->update(screenBounds);
			
			// look if it has found a letter now
			for(int j=0; j<letters.size(); j++){
				if( ofInsidePoly( *pos, letters[j].points ) ) {
					trail->letterId = j;
					trail->color.set(0xff, 0xff, 0xff);
				}
			}
		}
		else trail->update(letters[trail->letterId]);
		*/
		
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	fbo.begin();
	
	ofSetColor(255, 255, 255, 255);
	bg.draw(0, 0);
	
	int i;
	
	if(bDebug){
		ofSetColor(90, 90, 90);
		for(i=0; i<letters.size(); i++){
			letters[i].draw();
			//letters[i].drawNormals();
		}
	}
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	for(i=0; i<trails.size(); i++){
		trails[i].draw();
	}
	
	fbo.end();
	
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, ofGetHeight(), fbo.getWidth(), -fbo.getHeight());
	
	ofSetColor(0xff, 0xff, 0xff);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), ofGetWidth()-40, ofGetHeight()-10);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	if (prevMouse.x!=0 && prevMouse.y !=0) {
		ofPoint mouse = ofPoint(x, y);
		trails.push_back( Trail( ofPoint(x, y), prevMouse-mouse ) );
		
		if(trails.size()>MAX_TRAILS){
			trails.erase(trails.begin(), trails.begin()+(trails.size()-MAX_TRAILS));
		}
		
	}
	
	prevMouse.set(mouseX, mouseY);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	
	
	switch (key) {
		case 'd':
			bDebug = !bDebug;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}