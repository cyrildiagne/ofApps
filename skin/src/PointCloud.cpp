/*
 *  UserPointCloud.cpp
 *  skin
 *
 *  Created by kikko on 2/24/11.
 *  Copyright 2011 Cyril Diagne. All rights reserved.
 *
 */

#include "PointCloud.h"

PointCloud::PointCloud() {
	
	allocated = false;
	
	numUserPixels=0;
	
	//shader.setup("userpointcloud");
}

PointCloud::~PointCloud() {
	
	if(allocated) {
		//delete[] vertices;
	}
}

void PointCloud::init(ofxDepthGenerator& depth, ofxImageGenerator& image, ofxUserGenerator& user, int type) {
	
	this->depth = &depth;
	this->user = &user;
	this->image = &image;
	this->type = type;
	
}

void PointCloud::update() {
		
	const XnDepthPixel* depthPixels = depth->getXnDepthGenerator().GetDepthMap();
	const XnUInt8* imagePixels = image->getXnImageGenerator().GetImageMap();
	
	int texture=0;
	bool bUserInScene = user->getTrackedUsers().size()>0;
	
	numUserPixels=0;
	
	if (type==POINTCLOUD_SCENE || type==POINTCLOUD_BG && !bUserInScene) {
		
		for (int y=0; y < 480; y++) { 
			for (int x=0; x < 640; x++, depthPixels++, texture+=3){
								
				projective[numUserPixels].X = x; 
				projective[numUserPixels].Y = y;
				projective[numUserPixels].Z = *depthPixels;
				
				colors[numUserPixels].r = imagePixels[texture] / 255.f;
				colors[numUserPixels].g = imagePixels[texture+1] / 255.f;
				colors[numUserPixels].b = imagePixels[texture+2] / 255.f;
					
				numUserPixels++;
			}
		}
	}
	else {
		
		if(!bUserInScene) return;
		
		SceneMetaData smd;
		user->getXnUserGenerator().GetUserPixels(0, smd);
		unsigned short * sceneDepth = (unsigned short*)smd.Data();
		
		for (int y=0; y < 480; y++) { 
			for (int x=0; x < 640; x++, depthPixels++, texture+=3){
				
				int i = y*640+x;
				
				bool bValid = sceneDepth[i] != 0;
				if (type==POINTCLOUD_BG) bValid = !bValid;
				
				if (bValid) {
					
					projective[numUserPixels].X = x; 
					projective[numUserPixels].Y = y;
					projective[numUserPixels].Z = *depthPixels;
					
					colors[numUserPixels].r = imagePixels[texture] / 255.f;
					colors[numUserPixels].g = imagePixels[texture+1] / 255.f;
					colors[numUserPixels].b = imagePixels[texture+2] / 255.f;
					
					numUserPixels++;
				}
			}
		}
	}
	
	numUserPixels--;
	
	// get realworlds positions
	
	depth->getXnDepthGenerator().ConvertProjectiveToRealWorld(numUserPixels, projective, realworld);
	
	for(int i=0; i<numUserPixels; i++) {
		vertices[i].x = realworld[i].X;
		vertices[i].y = realworld[i].Y;
		vertices[i].z = realworld[i].Z;
	}
	
	vbo.setVertexData(vertices, numUserPixels, GL_DYNAMIC_DRAW);
	vbo.setColorData(colors, numUserPixels, GL_STATIC_DRAW);	
	
	allocated = true;
}

void PointCloud::draw() {
	
	if(!allocated) return;
	
	vbo.draw(GL_POINTS, 0, numUserPixels);
}