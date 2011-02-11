/*
 *  Letter.h
 *  holler_logo_1
 *
 *  Created by kikko on 2/2/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofShape.h"
#include "ofShapeUtils.h"

class Border : public ofPolyline {

public:
	
	Border(){};
	Border(vector<ofPoint> &pts, float spacing=NULL){ set(pts, spacing); }
	
	void set(vector<ofPoint> &pts, float spacing=NULL);
	void draw();
	void drawNormals();
	
	int checkCollision(ofPoint position, ofPoint velocity, ofVec2f* newPosition, ofVec2f* newVelocity, vector<ofPoint>* isectPoints);
	float checkIntersection(ofVec2f p1, ofVec2f s1, ofVec2f p2, ofVec2f s2);

	vector<ofVec2f> edges;
	vector<ofVec2f> normals;
	
	ofColor color;
	
	void addHole(Border *hole);
	
private:
	
	void computeNormals();
};