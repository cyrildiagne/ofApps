#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	logoWhite.loadImage("logo_white.png");
	
	bg.loadImage("bg_dark.png");
	
	hollerLogoApp::setup();
	
	fbo.begin();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	fbo.end();
	
	for (int i=0; i<letters.size(); i++) {
		numTrailsInside.push_back(0);
	}
}

//--------------------------------------------------------------
void testApp::resetApp(){
	trails.clear();
	
	fbo.begin();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	fbo.end();
}

//--------------------------------------------------------------
void testApp::updateApp(){
	
	for(int i=0; i<trails.size(); i++){
		trails[i].update();
	}
}

//--------------------------------------------------------------
void testApp::drawApp(){
	
	ofPoint pt;
	ofPoint middle;
	ofPoint pt2;
	int i, j;
	
	fbo.end();
	ofSetColor(255, 255, 255, 255);
	bg.draw(0, 0);
	
	vector<ofPoint> pts;
	
	// TODO : replace drawing method with png
	ofFill();
	for(i=0; i<numTrailsInside.size(); i++){
		ofSetColor(0,0, 0, numTrailsInside[i]*3);
		ofBeginShape();
		ofVertexes(letters[i].getVertices());
		ofEndShape(false);
		
		numTrailsInside[i] = 0;
	}	
	
	fbo.begin();
	
	ofEnableAlphaBlending();
	
	ofSetColor(255, 255, 255, 4);
	bg.draw(0, 0);
	
	// TODO : replace drawing method with png
	ofFill();
	for(i=0; i<numTrailsInside.size(); i++){
		ofSetColor(0, 0, 0, numTrailsInside[i]*12);
		ofBeginShape();
		ofVertexes(letters[i].getVertices());
		ofEndShape(false);
		
		numTrailsInside[i] = 0;
	}
	
	
	ofColor c1, c2, c3;
	
	ofSetLineWidth(1);
	
	for (i=0; i<trails.size(); i++){
		
		pt = trails[i].position;
		
		if(ofInsidePoly(pt, trails[i].bounds->getVertices())){
			
			c1 = ofColor(0, 0, 0) / 255;	c1.a = 0.7;
			c2 = ofColor(HRED_R, HRED_G, HRED_B)  / 255;	c2.a = 1;
			c3 = ofColor(0, 0, 0) / 255;	c3.a = 1;
			numTrailsInside[trails[i].letterId] += 1;
			
		} else {
			
			c1 = ofColor(HRED_R, HRED_G, HRED_B)  / 255;	c1.a = 1;
			c2 = ofColor(0, 0, 0) / 255;	c2.a = 0.6;
			c3 = ofColor(HRED_R, HRED_G, HRED_B-20)  / 255;	c3.a = 0;
		}
		
		
				
		ofSetColor(125, 0, 0, 255);
		
		pts = trails[i].bounds->getVertices();
		
		for (j=0; j<pts.size(); j++){
			
			pt2 = pts[j];
			middle = pt.getMiddle(pt2);
			
			if(pt.squareDistance(pt2) < 3000){
				
				linePoints[0][0] = pt.x;
				linePoints[0][1] = pt.y;
				linePoints[0][2] = pt.z;
				
				linePoints[1][0] = middle.x;
				linePoints[1][1] = middle.y;
				linePoints[1][2] = middle.z;
				
				linePoints[2][0] = pt2.x;
				linePoints[2][1] = pt2.y;
				linePoints[2][2] = pt2.z;
				
				colors[0][0] = c1.r;
				colors[0][1] = c1.g;
				colors[0][2] = c1.b;
				colors[0][3] = c1.a;
				
				colors[1][0] = c2.r;
				colors[1][1] = c2.g;
				colors[1][2] = c2.b;
				colors[1][3] = c2.a;
				
				colors[2][0] = c3.r;
				colors[2][1] = c3.g;
				colors[2][2] = c3.b;
				colors[2][3] = c3.a;
				
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_FLOAT, 0, &colors[0][0]);
				
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, &linePoints[0][0]);
				
				glDrawArrays(GL_LINE_STRIP, 0, 3);
				
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				
			}
		}
		
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		
		ofSetColor(255, 200, 200, 20);
		
		for (j=0; j<trails.size(); j++){
			if(pt.squareDistance(trails[j].position) < 2000){
				ofLine(pt, trails[j].position);
			}
		}
		
		trails[i].draw();
		
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	}
}

void testApp::mouseDragged(int x, int y, int button){
	
	int num;
	
	do {
		num = (int)ofRandom(letters.size());
	} while (num==LETTER_E_INLINE || num==LETTER_O_INLINE);
		
	trails.push_back( Trail( ofPoint(x, y), ofPoint(-1, -1).getNormalized(), &letters[num], num ) );
	
	if(trails.size()>180){
		trails.erase(trails.begin());
	}
}