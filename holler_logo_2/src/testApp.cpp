#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground( ofColor(0, 0, 0) );
	//ofSetBackgroundAuto(false);
	ofSetFrameRate(60);
	
	logo.loadImage("logo.png");
	logo.setImageType(OF_IMAGE_GRAYSCALE);
	
	logoWhite.loadImage("logo_white.png");
	
	bg.loadImage("bg.png");
	
	fbo.setup(ofGetWidth(), ofGetHeight(), GL_RGBA, 1);
	
	bDebug = false;
	
	setScreenBounds();
	
    loadContours();	
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
	contourFinder.findContours(logoCv, 50*50, 300*300, 10, true, true);
		
	for(int i=0; i<contourFinder.blobs.size(); i++){
		letters.push_back( Border(contourFinder.blobs[i].pts, 10.f) );
	}
	
	//letters[LETTER_O_OUTLINE].addHole(&letters[LETTER_O_INLINE]);
	//letters[LETTER_E_OUTLINE].addHole(&letters[LETTER_E_INLINE]);
}

//--------------------------------------------------------------
void testApp::update(){
	
	ofPoint * pos;
	Trail *trail;
	
	bool bInsideLetter;
	
	vector<ofPoint> growings;
	
	for(int i=0; i<trails.size(); i++){
		
		trail = &trails[i];
		pos = &trail->position;
		
		// If trail isn't in a letter yet
		if(trail->letterId == -1) {
			
			trail->update(screenBounds);
			
			if(!ofInsidePoly( trail->position, screenBounds.points )){
				trails.erase(trails.begin()+i);
				i--;
				continue;
			}
			
			// look if it has found a letter now
			for(int j=0; j<letters.size(); j++){
				if( ofInsidePoly( *pos, letters[j].points ) ) {
					trail->letterId = j;
				}
			}
		}
		else {
			
			trail->update(letters[trail->letterId]);
			
			//if (trails[i].bPlantMode) {
				growings.push_back(trails[i].position);
			//}
		}
	}
	
	triangle.clear();
	if(growings.size()>=3)
		triangle.triangulate( growings );
}

//--------------------------------------------------------------
void testApp::draw(){
	
	fbo.begin();
	
	ofEnableAlphaBlending();
	
	ofSetColor(255, 255, 255);
	bg.draw(0, 0);
	
	int i, j;
	
	if(bDebug){
		ofSetColor(90, 90, 90);
		for(i=0; i<letters.size(); i++){
			letters[i].draw();
			//letters[i].drawNormals();
		}
	}
	
	
	for(i=0; i<trails.size(); i++) {
		
		trails[i].draw();
	}
	
	vector<ofxTriangleData> triangles = triangle.getTriangles();
	for (int i=0; i<triangles.size(); i++) {
		
		ofSetColor(255, 255, 255, 80);
		ofNoFill();
		ofTriangle(triangles[i].a, triangles[i].b, triangles[i].c);
		//ofSetColor(255, 255, 255, 80);
		//ofFill();
		//ofTriangle(triangles[i].a, triangles[i].b, triangles[i].c);
	}
	
	fbo.end();
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
	fbo.draw(0, ofGetHeight(), fbo.getWidth(), -fbo.getHeight());
	
	ofSetColor(255, 255, 255);
	logoWhite.draw(0, 0);
	
	ofSetColor(0xff, 0xff, 0xff);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), ofGetWidth()-40, ofGetHeight()-10);
	
	mouseRecord.draw();
}


void testApp::mouseDragged(int x, int y, int button){
	
	trails.push_back( Trail( ofPoint(x, y), ofPoint(-1, -1).getNormalized() ) );
	if(trails.size()>800){
		trails.erase(trails.begin());
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	switch (key) {
		case 'd':
			bDebug = !bDebug;
			break;
		case 'm': mouseRecord.toggleRecording(); break;
		case 'p': mouseRecord.togglePlaying(); break;
			
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}