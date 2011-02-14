/*
 *  Trail.cpp
 *  holler_logo_1
 *
 *  Created by kikko on 2/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Trail.h"

Trail::Trail(ofPoint initPos, ofPoint initVelocity, Border* bounds, int letterId){
	
	position = initPos;
	velocity = ofPoint();
	this->bounds = bounds;
	this->letterId = letterId;
	
	color.set(114, 150+ofRandom(80)-40, 52);
	
	letterId = -1;
	
	target = getNewTarget();
}

void Trail::update(){
		
	/*acc += steerTo(target);
	
	velocity += acc;
	velocity.limit(maxspeed);
	position += velocity;
	
	acc *= 0;*/
	
	position += (target-position)*0.02;
	
	// assign new target if distance to target is smaller than 10
	if( (target - position).squareLength() < 10 ) {
		
		target = getNewTarget();
	}
}

void Trail::draw(){
	
	ofFill();
	
	ofSetColor(255, 255, 255, 255);
	ofCircle(position.x, position.y, 2);
	
	ofSetColor(255, 255, 255, 50);
	ofCircle(position.x, position.y, 8);
}

ofPoint Trail::getNewTarget(){
	
	int num = (int)ofRandom(bounds->points.size());
	ofPoint pos = bounds->points[ num ];
	
	//return pos;
	int amp = 50;
	return ofPoint(pos.x + ofRandom(-amp, amp), pos.y + ofRandom(-amp, amp) );
}