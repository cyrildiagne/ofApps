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
	
	mesh.primitives.push_back(ofPrimitive());
	data = &mesh.primitives[0];
	data->setMode(OF_TRIANGLES_MODE);
	
	setMesh(&mesh);
	setRenderMethod(OF_MESH_USING_VBO);
	setDrawType(GL_DYNAMIC_DRAW);
	
	
	shaderFile = "shaders/pointcloud";
}

PointCloud::~PointCloud() {
	
	
}

void PointCloud::init(ofxDepthGenerator& depth, ofxImageGenerator& image, ofxUserGenerator& user, int type) {
	
	this->depth = &depth;
	this->user = &user;
	this->image = &image;
	this->type = type;
	
	img.loadImage("test.jpg");
	
	shader.setup(shaderFile);
}

void PointCloud::update() {
		
	const XnDepthPixel* depthPixels = depth->getXnDepthGenerator().GetDepthMap();
	const XnUInt8* imagePixels = image->getXnImageGenerator().GetImageMap();
	
	int texture=0;
	bool bUserInScene = user->getTrackedUsers().size()>0;
	
	numUserPixels=0;
	
	data->clear();
	
	if (type==POINTCLOUD_SCENE || type==POINTCLOUD_BG && !bUserInScene) {
		
		for (int y=0; y < 480; y++) { 
			for (int x=0; x < 640; x++, depthPixels++, texture+=3){
				
				int i = y*640+x;
				
				projective[numUserPixels].X = x; 
				projective[numUserPixels].Y = y;
				projective[numUserPixels].Z = *depthPixels;
				
				//data->addColor(ofColor(imagePixels[texture]/255.f, imagePixels[texture+1]/255.f, imagePixels[texture+2]/255.f));
				
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
					
					//data->addColor(ofColor(imagePixels[texture]/255.f, imagePixels[texture+1]/255.f, imagePixels[texture+2]/255.f));
					
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
		data->addVertex(ofPoint(pt->X, pt->Y, pt->Z));
		data->addTexCoord(ofVec2f(projective[i].X, projective[i].Y));
		
		if(!isUserCloud) {
			
			int x = (int)projective[i].X;
			int y = (int)projective[i].Y;
			int pos = y*640+x;
			
			if(pos>640*480-640-1) continue;
			
			if( realworld[pos].Z!=0 && realworld[pos+1].Z!=0 && realworld[pos+640+1].Z!=0) {
			
			data->addIndex(i);
			data->addIndex(i+1);
			data->addIndex(i+640+1);			
			
			data->addIndex(i);
			data->addIndex(i+640+1);
			data->addIndex(i+640);
		}
	}
}

void PointCloud::draw() {
	
	//enableColors();
	
	enableTexCoords();
	enableIndices();
	
	shader.begin();
	
	int texCoordAttLoc = shader.getAttributeLocation("aTextureCoords");
	glEnableVertexAttribArray(texCoordAttLoc);
	glVertexAttribPointer(texCoordAttLoc, 2, GL_FLOAT, false, 0, data->getTexCoordsPointer());
	glBindAttribLocation(shader.getProgram(), texCoordAttLoc, "aTextureCoords");
	
	//img.getTextureReference().bind();
	int texLoc = glGetUniformLocation(shader.getProgram(), "tex");
	shader.setUniformTexture("tex", img.getTextureReference(), texLoc);
	
	ofMeshRenderer::draw(&mesh, OF_MESH_FILL);
	
	//img.getTextureReference().unbind();
	
	shader.end();
}