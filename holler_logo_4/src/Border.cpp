/*
 *  Border.cpp
 *  holler_logo_1
 *
 *  Created by kikko on 2/2/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Border.h"

void Border::set(vector<ofPoint> &pts, float spacing){
	
	if(spacing!=NULL){
		
		ofPolyline temp;
		temp.addVertexes(pts);
		temp = ofGetResampledSpacing(temp, spacing);
		addVertexes(temp.points);
	}
	else {
		addVertexes(pts);
	}
	
	color.set(ofRandom(255), ofRandom(255), ofRandom(255));
	
	computeNormals();
	computeBounds();
}

void Border::computeNormals(){

	int i;
	for (i=0; i<points.size()-1; i++) {
		edges.push_back( ofVec2f(points[i+1]-points[i]));
		normals.push_back( ofVec2f(points[i+1]-points[i]).getPerpendicular() );
	}
	edges.push_back( ofVec2f(points[0]-points[i]));
	normals.push_back( ofVec2f(points[0]-points[i]).getPerpendicular() );
}

void Border::computeBounds(){
	
	bounds.x        = 100000;
	bounds.y	    = 100000;
	bounds.width    = 0;
	bounds.height   = 0;
	
	float farRight  = -100000;
	float bottom    = -100000;
	
	for (int i=0; i<points.size(); i++) {
		if( points[i].x < bounds.x ) bounds.x = points[i].x;	
		if( points[i].y < bounds.y ) bounds.y = points[i].y;
		if( points[i].x > farRight ) farRight = points[i].x; 
		if( points[i].y > bottom   ) bottom   = points[i].y; 
	}
	
	bounds.width  = ABS(farRight - bounds.x);
	bounds.height = ABS(bottom - bounds.y); 
}

void Border::addHole(Border * hole){
	
	points.insert( points.end(), hole->points.begin(), hole->points.end() );
	edges.insert( edges.end(), hole->edges.begin(), hole->edges.end() );
	normals.insert( normals.end(), hole->normals.begin(), hole->normals.end() );
}


// code ported from Johan Van Mol 's Collision detection & boucing balls
// http://www.johanvanmol.org/content/view/45/37/1/6/



/** checkCollision
* return : the number of collisions
* @position : position of the particle
* @velocity : velocity of the particle
* @newPosition : result position after the collision(s)
* @newVelocity : result velocity after the collision(s)
* @isectPoints : pointer to a vertor of point to store collisions positions
*/
int Border::checkCollision(ofPoint position, ofPoint velocity, ofVec2f* newPosition, ofVec2f* newVelocity, vector<ofPoint>* isectPoints){
	
	bool bCollide = false;
	ofPoint isectPoint;
	
	for (int i=0; i<points.size(); i++) {
		
		// verify if we're moving toward this wall's normal
		if(normals[i].dot(velocity) >= 0) continue;
		
		float result = checkIntersection(position, velocity, ofVec2f(points[i]), edges[i]);
		
		if(result!=-1) {
			
			isectPoint.set(position + velocity * result);
			isectPoints->push_back( isectPoint );
			
			// handle slopped wall :
			
			ofVec2f wallVec = edges[i].getNormalized();
			ofVec2f wallNormal = wallVec.getPerpendicular();
			
			// get N and T component of speed vector, invert tangential component
			
			float t = wallVec.dot(velocity);	// magnitude of the tangential component
			float n = wallNormal.dot(velocity); // magnitude of the normal component
			
			// turn into a vector again by multiplying the magnitude with 
			// the tangential and the (flipped) normal direction
			
			ofVec2f vt = wallVec * t;
			ofVec2f vn = wallNormal * -n;
			
			// project <vt, vn> onto x-y axis
			
			float newVx = ofVec2f(1, 0).dot(vn) + ofVec2f(1, 0).dot(vt);
			float newVy = ofVec2f(0, 1).dot(vn) + ofVec2f(0, 1).dot(vt);
			
			newVelocity->set(ofPoint(newVx, newVy));
			newPosition->set(isectPoint + *(newVelocity) * (1-result));
						
			bCollide = true;
			
			break;
		}
	}
	
	if(bCollide) {
		
		return 1 + checkCollision(*newPosition, *newVelocity, newPosition, newVelocity, isectPoints);
	}
	
	return 0;
}

float Border::checkIntersection(ofVec2f p1, ofVec2f s1, ofVec2f p2, ofVec2f s2) {
	
	float div = -s2.x * s1.y + s1.x * s2.y;
	float s = (-s1.y * (p1.x - p2.x) + s1.x * (p1.y - p2.y)) / div;
	float t = ( s2.x * (p1.y - p2.y) - s2.y * (p1.x - p2.x)) / div;
	
	if(t>=0 && t<=1 && s>=0 && s<=1){
		return t;
	} else {
		return -1;
	}
}

void Border::drawNormals(){
	
	ofSetLineWidth(1);
	
	ofPoint scaledNormal;
	
	for ( int i=0; i<(int)normals.size(); i++ ) {
		 
		scaledNormal = normals[i].getScaled(10.f);		 
		ofLine( points[i],
		ofPoint(points[i].x+scaledNormal.x, points[i].y+scaledNormal.y) );
	}
}

void Border::draw(){
	
	ofPolyline::draw();
}