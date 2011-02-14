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
	velocity = ofPoint();
	
	acc = initVelocity.getNormalized().getScaled(4);
	r = 2.0;
	wandertheta = initVelocity.angleRad(ofPoint(1, 0, 0)); //ofRandom(PI*2);
	maxforce = 0.1;
	maxspeed = 4.0;
	
	letterId = -1;
	
	color.set(180+ofRandom(70), 0, 0 );
	
	particle = Assets::Instance()->getImage(PARTICLE);
}

// ported from http://www.shiffman.net/itp/classes/nature/week06_s09/wander/Boid.pde
void Trail::wander(){
	
	float wanderR = 16.0f;         // Radius for our "wander circle"
    float wanderD = 60.0f;         // Distance for our "wander circle"
    float change = 0.25f;
	
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
	
	wander();
	
	velocity += acc;
	velocity.limit(maxspeed);
	position += velocity;
	
	acc *= 0;
	
	pts.push_back(position);
	
	while (pts.size() >= TRAIL_LENGTH) {
		pts.erase (pts.begin());
	}
}

void Trail::update(Border & bounds) {
	
	ofVec2f newVelocity;
	ofVec2f newPosition;
	
	int numCollision = bounds.checkCollision(position, velocity, &newPosition, &newVelocity, &pts);
	
	if(numCollision>0) {
		
		position = newPosition;
		velocity = newVelocity.getNormalized().getScaled(4);
	}
	else {
		
		update();
	}
}

void Trail::draw(){
		
	ofSetLineWidth(1);
	
	ofEnableAlphaBlending();
	
	float alpha = (float)color.a/255;
	
	int pos = pts.size()*0.8;
	
	for(int i=1; i<pts.size(); i++){
		float offset = i > pos ? 255 * (float)(i-pos)/(pts.size()-pos) : 1;
		ofSetColor(color.r + offset, color.g + offset, color.b +offset, alpha * (float)i/TRAIL_LENGTH*255);
		ofLine(pts[i-1].x, pts[i-1].y, pts[i].x, pts[i].y);
	}
	
	ofSetColor(color.r, color.g, color.b, alpha * 70);
	ofFill();
	ofCircle(pts.back().x, pts.back().y, 5);
	//particle->draw(pts.back().x-5, pts.back().y-4);
	
	ofSetColor(color.r, color.g, color.b, alpha * 149);
	ofNoFill();
	ofSetLineWidth(1);
	ofCircle(pts.back().x, pts.back().y, 5);
	
	ofDisableAlphaBlending();
}