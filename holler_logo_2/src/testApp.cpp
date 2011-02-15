#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	logoWhite.loadImage("logo_white.png");
	bg.loadImage("bg_dark.png");
		
	hollerLogoApp::setup();
}



void testApp::resetApp(){
	trails.clear();
}

//--------------------------------------------------------------
void testApp::updateApp(){
	
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
			
			if(!ofInsidePoly( trail->position, screenBounds.getVertices() )){
				trails.erase(trails.begin()+i);
				i--;
				continue;
			}
			
			// look if it has found a letter now
			for(int j=0; j<letters.size(); j++){
				if( ofInsidePoly( *pos, letters[j].getVertices() ) ) {
					trail->letterId = j;
				}
			}
		}
		else {
			
			trail->update(letters[trail->letterId]);
			
			if (trails[i].bActive) {
				growings.push_back(trails[i].position);
			}
		}
	}
	
	triangle.clear();
	if(growings.size()>=3)
		triangle.triangulate( growings, growings.size() );
}

//--------------------------------------------------------------
void testApp::drawApp(){
		
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
	
	ofFill();
	
	for(i=0; i<trails.size(); i++) {
		
		trails[i].draw();
	}
	
	ofSetLineWidth(0.5);
	vector<ofxTriangleData> triangles = triangle.getTriangles();
	ofSetColor(210, 210, 255, 190);
	//ofSetColor(HRED_R, HRED_G, HRED_G);
	ofNoFill();
	for (int i=0; i<triangles.size(); i++) {
		
		ofTriangle(triangles[i].a, triangles[i].b, triangles[i].c);
	}
	
	ofSetColor(255, 255, 255);
	logoWhite.draw(0, 0);
}


void testApp::mouseDragged(int x, int y, int button){
	
	trails.push_back( Trail( ofPoint(x, y), ofPoint(-1, -1).getNormalized() ) );
	if(trails.size()>400){
		trails.erase(trails.begin());
	}
}