/*
 *  LightenImage.h
 *  emptyExample
 *
 *  Created by Cyril Diagne on 19/04/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofVboMesh.h"

class LightenImage {

public:
	
	void setup();
	void draw(ofCamera& camera, ofLight& light);
	
	int width, height;
	
private:
	
	void setupMesh();
	
	ofVboMesh mesh;
	
	ofImage buddahColor;
    ofImage buddahNormals;
	
    ofShader shader;
};
