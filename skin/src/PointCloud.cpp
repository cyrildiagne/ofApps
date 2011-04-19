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
	
	numUserPixels=0;
	
	shaderFile = "shaders/pointcloud";
}

PointCloud::~PointCloud() {
	
	
}

void PointCloud::init(ofxONI & oni, int type) {
	
    this->oni = &oni;
	this->depth = &oni.depth;
	this->user = &oni.user;
	this->image = &oni.image;
    
	this->type = type;
    
	shader.setup(shaderFile);
}

void PointCloud::update() {
		
	const XnDepthPixel* depthPixels = depth->getXnDepthGenerator().GetDepthMap();
	const XnUInt8* imagePixels = image->getXnImageGenerator().GetImageMap();
	
	int texture=0;
	bool bUserInScene = user->getTrackedUsers().size()>0;
	
	numUserPixels=0;
	
	clear();
	
	if (type==POINTCLOUD_SCENE || type==POINTCLOUD_BG && !bUserInScene) {
		
		for (int y=0; y < 480; y++) { 
			for (int x=0; x < 640; x++, depthPixels++, texture+=3){
				
				int i = y*640+x;
				
				projective[numUserPixels].X = x; 
				projective[numUserPixels].Y = y;
				projective[numUserPixels].Z = *depthPixels;
				
				//addColor(ofColor(imagePixels[texture]/255.f, imagePixels[texture+1]/255.f, imagePixels[texture+2]/255.f));
				
				/*
				 faces.push_back(Face(	&vertices[pos],
										&vertices[pos+1],
										&vertices[pos+numVertsW+1]));
				 
				 faces.push_back(Face(	&vertices[pos],
										&vertices[pos+numVertsW+1],
										&vertices[pos+numVertsW]));
				 */
				
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
					projective[numUserPixels].Z = *depthPixels ;
					
                    //addColor(ofColor(imagePixels[texture]/255.f, imagePixels[texture+1]/255.f, imagePixels[texture+2]/255.f));
                    
					numUserPixels++;
				}
			}
		}
	}
	
	numUserPixels--;
	
	// get realworlds positions
	
	depth->getXnDepthGenerator().ConvertProjectiveToRealWorld(numUserPixels, projective, realworld);
	
	bool isUserCloud = !(type==POINTCLOUD_SCENE || type==POINTCLOUD_BG && !bUserInScene);
	
	XnPoint3D * pt;
	for(int i=0; i<numUserPixels; i++) {
		
		pt = &realworld[i];
		addVertex(ofPoint(pt->X, pt->Y, pt->Z));
		addTexCoord(ofVec2f(projective[i].X, projective[i].Y));
		
        /*
		if(!isUserCloud) {
			
			int x = (int)projective[i].X;
			int y = (int)projective[i].Y;
			int pos = y*640+x;
			
			if(pos>640*480-640-1) continue;
			
			if( realworld[pos].Z!=0 && realworld[pos+1].Z!=0 && realworld[pos+640+1].Z!=0) {
			
			addIndex(i);
			addIndex(i+1);
			addIndex(i+640+1);			
			
			addIndex(i);
			addIndex(i+640+1);
			addIndex(i+640);
		}
        */
	}
}

void PointCloud::draw() {
	
	shader.begin();
	
	int texCoordAttLoc = shader.getAttributeLocation("aTextureCoords");
	glEnableVertexAttribArray(texCoordAttLoc);
	glVertexAttribPointer(texCoordAttLoc, 2, GL_FLOAT, false, 0, getTexCoordsPointer());
	glBindAttribLocation(shader.getProgram(), texCoordAttLoc, "aTextureCoords");
    
    glActiveTexture(GL_TEXTURE0);
	oni->image.getTextureReference().bind();
	shader.setUniform1i("tex", 0);
    oni->image.getTextureReference().unbind();
    
	ofVboMesh::draw(OF_MESH_POINTS);
	
    shader.end();
}