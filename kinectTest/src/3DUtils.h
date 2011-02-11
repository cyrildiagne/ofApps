/*
 *  Vertex.h
 *  KinectBubble
 *
 *  Created by kikko on 11/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"


class Vertex {
public:
	
	Vertex(){
		vId = -1;
		pos = ofxVec3f();
	};
	
	Vertex(int indice, float x, float y, float z){
		vId = indice;
		pos = ofxVec3f(x, y, z);
	};
	
	int vId;
	ofxVec3f pos;
	ofxVec3f n;
	ofxVec3f uv;
};


class Face {
public:
	Face(Vertex *v1, Vertex *v2, Vertex *v3){
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;
	};
	Vertex * vertices[3];
};



inline void checkGLErrors(string prefix) {
	GLenum errCode;
	string *errString;
	errCode = glGetError();
	if(errCode != GL_NO_ERROR) {
		do  {
			string errString = (const char *)gluErrorString(errCode);
			ofLog(OF_LOG_ERROR, prefix+" "+errString+" (OpenGL)");
		}
		while ((errCode= glGetError()) != GL_NO_ERROR);
	}
	//else ofLog(OF_LOG_ERROR, prefix + " OpenGL statut OK");
}