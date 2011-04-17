/*
 *  UserPointCloud.h
 *  skin
 *
 *  Created by kikko on 2/24/11.
 *  Copyright 2011 Cyril Diagne. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofVboMesh.h"
#include "ofxOpenNI.h"
#include "Global.h"

using namespace xn;

#define POINTCLOUD_SCENE 1
#define POINTCLOUD_BG	 2
#define POINTCLOUD_USER	 3

class PointCloud : public ofVboMesh {
	
public:
	
	PointCloud();
	~PointCloud();
	
	void init(ofxONI& oni, int type);
	void update();
	void draw();
	
	int numUserPixels;
	int type;
	
	string shaderFile;
	
protected:
	
	ofxDepthGenerator * depth;
	ofxUserGenerator * user;
	ofxImageGenerator * image;
	
	ofShader shader;

private:
	
	XnVector3D	realworld	[640*480];
	XnVector3D	projective	[640*480];
};