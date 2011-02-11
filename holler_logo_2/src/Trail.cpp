/*
 *  Trail.cpp
 *  holler_logo_1
 *
 *  Created by kikko on 2/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Trail.h"

Trail::Trail(ofPoint initPos, ofPoint initVelocity){
	
	position = initPos;
	velocity = ofPoint();//initVelocity.getScaled(4);
	
	color.set(114, 150+ofRandom(80)-40, 52);
	//color.set(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
	
	r = 2.0;
	wandertheta = -initVelocity.angleRad(ofVec3f(0, 0));
	maxforce = 0.2;
	maxspeed = 8.0;
	
	letterId = -1;
	
	bPlantMode = false;
	bActive = true;
	
	particle = Assets::Instance()->getImage(LEAF);
	particle->setAnchorPoint(7, 21);
}

// ported from http://www.shiffman.net/itp/classes/nature/week06_s09/wander/Boid.pde
void Trail::wander(){
	
	float wanderR = 32.0f;         // Radius for our "wander circle"
    float wanderD = 80.0f;         // Distance for our "wander circle"
    float change = 0.20f;
	
	wandertheta += ofRandom(-change,change);     // Randomly change wander theta
	
    // Now we have to calculate the new location to steer towards on the wander circle
	
    ofPoint circleloc = velocity; // Start with velocity
    circleloc.normalize();	// Normalize to get heading
    circleloc *= wanderD;	// Multiply by distance
    circleloc += position;	// Make it relative to boid's location
    
    ofPoint circleOffSet = ofPoint(wanderR*cos(wandertheta), wanderR*sin(wandertheta));
   
	ofPoint target = circleloc + circleOffSet;
	acc += steer(target);  // Steer towards it
}

ofPoint Trail::steer(ofPoint target){
	
	ofPoint steer;  // The steering vector
	ofPoint desired = target - position;  // A vector pointing from the location to the target
	
	
	
	float d = desired.length(); // Distance from the target is the magnitude of the vector
		
	// If the distance is greater than 0, calc steering (otherwise return zero vector)
	if (d > 0) {
		// Normalize desired
		desired.normalize();
		desired *= maxspeed;
		
		// Steering = Desired minus Velocity
		steer = desired - velocity;
		steer.limit(maxforce);  // Limit to maximum steering force
		
	} else {
		steer = ofPoint(0,0);
	}
	return steer;
}

void Trail::update(){
	
	if(!bActive) {
		ofPoint rdm = ofPoint( ofRandom(-1,1), ofRandom(-1, 1) );
		float percent;
		for (int i=0; i<pts.size(); i++) {
			percent = (float)i/pts.size();
			pts[i].x += rdm.x * percent;
			pts[i].y += rdm.y * percent;
		}
		return;
	};
	
	wander();
	
	velocity += acc;
	velocity.limit(maxspeed);
	position += velocity;
	
	acc *= 0;
	
	if(bPlantMode) {
		
		maxspeed *= 0.97;
		maxforce *= 0.997;
		
		if((int)ofRandom(2)==1) {
			pts.push_back(position);
			//angles.push_back();
		}
		
		if(maxspeed < 2) bActive = false;
	}
}

void Trail::update(Border & bounds) {
	
	if(letterId!=-1 && !bPlantMode){
		bPlantMode = true;
		//angles.push_back
		color.set(0x50, 0x50, 0x38, 0xff);
	}
	
	update();
}

void Trail::draw(){
	
	//ofSetColor(color.r, color.g, color.b, color.a);
	
	float percent;
	
	for(int i=1; i<pts.size(); i++){
		
		percent = (float)i/pts.size();
		float offset = 255 * percent;
		
		ofSetColor(color.r+offset*0.5, color.g + offset*0.8, color.b+offset*0.3, 255);
		ofSetLineWidth( (1-percent) * 5 );
		ofLine(pts[i-1].x, pts[i-1].y, pts[i].x, pts[i].y);
	}
	
	
	float angle = velocity.angleRad(ofPoint(1, 0, 0));
	
	if(bPlantMode){
		if(bActive){
			
			ofPushMatrix();
			ofTranslate(position.x, position.y, 0);
			//ofSetColor(255, 255, 255, 50);
			//ofCircle(0, 0, 15);
			ofSetColor(255, 255, 255);
			ofCircle(0, 0, 3);
			//ofRotate( 90-ofRadToDeg( angle ) );
			//particle->draw(0, 0);
			/*ofFill();
			 ofSetColor(120, 190, 40, 140);
			 ofCircle(0, 0, 6);
			 ofSetColor(120, 190, 40, 50);
			 ofCircle(0, 0, 12);
			 ofSetColor(255, 255, 255, 255);
			 ofCircle(0, 0, 3);
			 */
			
			ofPopMatrix();
		}
		
	}
	else {
		
		
		
		ofFill();
		//ofSetColor(color.r, color.g, color.b, 255);
		ofSetColor(100, 160, 60, 255);
		ofCircle(position.x, position.y, 2);
	}
	
}