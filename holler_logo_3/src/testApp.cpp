#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground( ofColor(0, 0, 0) );
	ofSetFrameRate(60);
	
	logoWhite.loadImage("logo_white.png");
	bg.loadImage("bg.png");
	
	box2d.init();
	box2d.createBounds();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	hollerLogoApp::setup();
	
	addLettersToBox2D();
}


void testApp::resetApp(){
	
	lettersImg.clear();
	
	int i;
	
	for (i=0; i<joints.size(); i++) {
		joints[i].destroy();
	}
	joints.clear();
	
	for (i=0; i<circles.size(); i++) {
		circles[i].destroy();
	}
	circles.clear();
	
	for (i=0; i<lettersPoly.size(); i++) {
		lettersPoly[i].destroy();
	}
	lettersPoly.clear();
	
	anchor.destroy();
	
	
	addLettersToBox2D();
}

void testApp :: addLettersToBox2D(){
	
	anchor.setPhysics(0.0, 0.4, 0.3);
	anchor.setup(box2d.getWorld(), ofGetWidth()*0.5, 100, 400, 12);
	
	for (int i=0; i<letters.size(); i++) {
		
		if(i==LETTER_O_INLINE || i==LETTER_E_INLINE) continue;
		
		ofxBox2dPolygon b2poly;
		
		for (int j=0; j<letters[i].points.size(); j++) {
			b2poly.addVertex(ofVec2f(letters[i].points[j]));
		}
		
		b2poly.triangulate();
		b2poly.create(box2d.world);
		offsets.push_back(getOffset(b2poly));
		
		ofxBox2dJoint j1, j2;
		
		ofImage img;
		
		switch (i) {
			case LETTER_H:
				setupJoint(j1, b2poly, anchor, 303, 332, 303, 100);
				setupJoint(j2, b2poly, anchor, 415, 332, 415, 100);
				img.loadImage("img/h.png");
				break;
			case LETTER_O_OUTLINE:
				setupJoint(j1, b2poly, anchor, 498, 332, 498, 100);
				setupJoint(j2, b2poly, anchor, 590, 332, 590, 100);
				img.loadImage("img/o.png");
				break;
			case LETTER_O_ACCENT:
				setupJoint(j1, b2poly, anchor, 520, 270, 520, 100);
				setupJoint(j2, b2poly, anchor, 560, 270, 560, 100);
				img.loadImage("img/o_accent.png");
				break;
			case LETTER_L:
				setupJoint(j1, b2poly, anchor, 646, 251, 646, 100);
				setupJoint(j2, b2poly, anchor, 671, 251, 671, 100);
				img.loadImage("img/l.png");
				break;
			case LETTER_L2:
				setupJoint(j1, b2poly, anchor, 709, 251, 709, 100);
				setupJoint(j2, b2poly, anchor, 732, 251, 732, 100);
				img.loadImage("img/l2.png");
				break;
			case LETTER_E_OUTLINE:
				setupJoint(j1, b2poly, anchor, 786, 332, 786, 100);
				setupJoint(j2, b2poly, anchor, 885, 332, 885, 100);
				img.loadImage("img/e.png");
				break;
			case LETTER_R:
				setupJoint(j1, b2poly, anchor, 940, 332, 940, 100);
				setupJoint(j2, b2poly, anchor, 1003, 332, 1003, 100);
				img.loadImage("img/r.png");
				break;
		}
		
		lettersImg.push_back(img);
		
		joints.push_back(j1);
		joints.push_back(j2);
		
		lettersPoly.push_back(b2poly);
	}
}

ofVec2f testApp::getOffset(ofxBox2dPolygon &poly){
	
	int minX=ofGetWidth(), minY=ofGetHeight();
	
	for (int i=0; i<poly.vertices.size(); i++) {
		if(poly.vertices[i].x < minX) {
			minX = poly.vertices[i].x;
		}
		if(poly.vertices[i].y < minY) {
			minY = poly.vertices[i].y;
		}
	}
		
	return ofVec2f(minX, minY)-poly.getCenter();
}

b2Vec2 testApp::getB2Vec(float x, float y) {
	
	return b2Vec2((float)x/OFX_BOX2D_SCALE, (float)y/OFX_BOX2D_SCALE);
}

void testApp::setupJoint(ofxBox2dJoint &joint, ofxBox2dBaseShape &s1, ofxBox2dBaseShape &s2, float p1x, float p1y, float p2x, float p2y) {
	joint.setup(box2d.getWorld(), s1.body, s2.body, getB2Vec(p1x, p1y), getB2Vec(p2x, p2y), .3, 1.0);
}

//--------------------------------------------------------------
void testApp::updateApp(){
	
	box2d.update();
}

//--------------------------------------------------------------
void testApp::drawApp(){
	
	ofSetColor(255, 255, 255);
	bg.draw(0, 0);
		
	ofSetColor(255, 255, 255);
	//logoWhite.draw(0, 0);
	
	int i;
	
	ofSetColor(190, 190, 190);
	for (i=0; i<joints.size(); i++){
		joints[i].draw();
	}
	
	anchor.draw();
	
	if(bDebug){
		
		ofNoFill();
		ofSetColor(0, 0, 0);
		
		for (i=0; i<lettersPoly.size(); i++){
			lettersPoly[i].draw();
		}
		for (i=0; i<circles.size(); i++){
			circles[i].draw();
		}
	} 
	else {
		
		ofFill();
		
		ofEnableAlphaBlending();
		
		ofVec2f pos;
		float radius;
		for (i=0; i<circles.size(); i++){
			radius = circles[i].getRadius();
			pos = circles[i].getPosition();
			ofSetColor(60, 100, 155, 120);
			ofCircle(pos.x, pos.y, radius);
			ofSetColor(20, 20, 20, 200);
			ofCircle(pos.x, pos.y, 2);
		}
		
		ofSetColor(255, 255, 255);
		
		for(i=0; i<lettersImg.size(); i++){
			
			ofPushMatrix();
			ofVec2f pos = lettersPoly[i].getCenter();
			ofTranslate(pos.x, pos.y, 0);
			ofRotate(lettersPoly[i].getRotation());
			ofTranslate(offsets[i].x, offsets[i].y, 0);
			lettersImg[i].draw(0, 0);
			ofPopMatrix();
		}
		
		ofDisableAlphaBlending();
	}
}

void testApp::keyPressed(int key){
	if(key == 'i'){
	//	mouseRecord.record("mouse_record.xml");
	}
}

void testApp::mouseDragged(int x, int y, int button){
	
	if(!box2d.mouseJoint) {
		ofxBox2dCircle circle;
		circle.setPhysics(1, 0.4, 0.3);
		circle.setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), ofRandom(10, 25));
		circle.addForce(ofVec2f(0, -5), 120);//setDamping(0, -10);
		//circle.addImpulseForce(ofVec2f(0, -70.0), ofVec2f(circle.bodyDef.position.x, circle.bodyDef.position.x));//.addForce(ofVec2f(-1, 0), 5);
		circles.push_back(circle);
	}
}