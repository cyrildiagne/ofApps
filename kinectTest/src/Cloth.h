/*
 *  Cloth.h
 *  KinectBubble
 *
 *  Created by kikko on 11/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "3DUtils.h"
#include "ofxKinect.h"

class Cloth {

public:
	
	Cloth(){};
	~Cloth();
	
	void init(int width=640, int height=480, int numW=10, int numH=10);
	void draw();
	void update(float * distancePixels);
	
	int width, height, numW, numH;
	float rotX, rotY;
	
    ofShader shader;
	
protected:
	
	void createGeometry();
	void updateBufferData();
	void computeNormals();
	void setupScreenForLesson();
	void setMatrixUniforms();
		
	GLfloat pMatrix[16];
	GLfloat mvMatrix[16];
	
	vector<Face> faces;
	vector<Vertex> vertices;
	
	GLfloat * normalsData;
	GLfloat * posData;
	GLuint * facesData;
	
	int facesSize;
	int verticesSize;
	
	GLuint verticesBuffer;
	GLuint facesBuffer;
	
	GLint posAttribLoc;
	GLint normalAttribLoc;
	GLint pMatrixUniformLoc;
	GLint mvMatrixUniformLoc;
	GLint nMatrixUniformLoc;
	
};