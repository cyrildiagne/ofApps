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
#include "ofModel.h"
#include "ofxOpenNI.h"
#include "Global.h"

using namespace xn;

#define POINTCLOUD_SCENE 1
#define POINTCLOUD_BG	 2
#define POINTCLOUD_USER	 3

class PointCloud : public ofMeshRenderer {
	
public:
	
	PointCloud();
	~PointCloud();
	
	void init(ofxDepthGenerator& depth, ofxImageGenerator& image, ofxUserGenerator& user, int type);
	void update();
	void draw();
	
	int numUserPixels;
	int type;
	
	string shaderFile;
	
	ofImage img;
	
protected:
	
	ofxDepthGenerator * depth;
	ofxUserGenerator * user;
	ofxImageGenerator * image;

	ofMesh mesh;
	ofPrimitive * data;
	
	ofShader shader;

private:
	
	XnVector3D	realworld	[640*480];
	XnVector3D	projective	[640*480];
};