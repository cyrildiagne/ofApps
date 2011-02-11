/*
 *  Cloth.cpp
 *  KinectBubble
 *
 *  Created by kikko on 11/27/10.
 *  Copyright 2010 lab212. All rights reserved.
 *
 */

#include "Cloth.h"

void Cloth::init(int width, int height, int numW, int numH) {
		
	this->width = width;
	this->height = height;
	this->numW = numW;
	this->numH = numH;
	
	rotX = 0; rotY = 0;
		
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	
	ofDisableArbTex();
	
	shader.setup("shaders/shader1.vert", "shaders/shader1.frag");
	
	checkGLErrors("init()");
	
	shader.begin();
	posAttribLoc		= glGetAttribLocationARB((GLhandleARB)shader.getProgram(), "aVertexPosition");
	normalAttribLoc		= glGetAttribLocationARB((GLhandleARB)shader.getProgram(), "aVertexNormal");
	glEnableVertexAttribArray(posAttribLoc);
	glEnableVertexAttribArray(normalAttribLoc);
	pMatrixUniformLoc	= glGetUniformLocationARB((GLhandleARB)shader.getProgram(), "uPMatrix");
	mvMatrixUniformLoc	= glGetUniformLocationARB((GLhandleARB)shader.getProgram(), "uMVMatrix");
	nMatrixUniformLoc	= glGetUniformLocationARB((GLhandleARB)shader.getProgram(), "uNMatrix");
	shader.end();
	
	glGenBuffersARB(1, &verticesBuffer);
	glGenBuffersARB(1, &facesBuffer);
	
	createGeometry();
}

Cloth::~Cloth() {
	glDeleteBuffers(1, &verticesBuffer);
	glDeleteBuffers(1, &facesBuffer);
}

void Cloth::createGeometry() {
	
	int i, j, pos;
	int numVertsW = numW+1;
	
	// create vertices
	
	float faceW = (float)width/numW;
	float faceH = (float)height/numH;
	for(i=0; i<numH+1; i++) {
		for(j=0; j<numW+1; j++) {
			pos = numVertsW*i+j;
			vertices.push_back(Vertex(pos, faceW*j, faceH*i, 0.f));
		}
    }
	
	// create faces
	
	for(i=0; i<numH; i++) {
		for(j=0; j<numW; j++) {
			pos = numVertsW*i+j;
			
			faces.push_back(Face(&vertices[pos],
								 &vertices[pos+1],
								 &vertices[pos+numVertsW+1]));
			
			faces.push_back(Face(&vertices[pos],
								 &vertices[pos+numVertsW+1],
								 &vertices[pos+numVertsW]));
			
		}
    }
	
	verticesSize = vertices.size();
	facesSize = faces.size();
	
	// compute normals
	
	computeNormals();
	
	// init buffers
	
	posData = new GLfloat[verticesSize*3]; // 3 coords per vertex
	normalsData = new GLfloat[verticesSize*3]; // 3 coords per vertex
	
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesSize*3*sizeof(GLfloat)*2, 0, GL_STREAM_DRAW);
	
	updateBufferData();
	
	facesData = new GLuint[faces.size()*3]; // 3 vertices per face
	for (int i=0; i<faces.size(); i++) {
		facesData[i*3]	 = faces[i].vertices[0]->vId;
		facesData[i*3+1] = faces[i].vertices[1]->vId;
		facesData[i*3+2] = faces[i].vertices[2]->vId;
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, facesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, facesSize*3*sizeof(GLuint), facesData, GL_STATIC_DRAW);
}

void Cloth::updateBufferData(){
	
	int i;
	
	// vertices & normals
	for (i=0; i<vertices.size(); i++) {
		
		posData[i*3] = vertices[i].pos.x;
		posData[i*3+1] = vertices[i].pos.y;
		posData[i*3+2] = vertices[i].pos.z;
		
		normalsData[i*3] = vertices[i].n.x;
		normalsData[i*3+1] = vertices[i].n.y;
		normalsData[i*3+2] = vertices[i].n.z;
	}
	
	int posDataSize = verticesSize*3*sizeof(GLfloat);
	int normalDataSize = verticesSize*3*sizeof(GLfloat);
	
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, posDataSize, posData);                            
	glBufferSubData(GL_ARRAY_BUFFER, posDataSize, normalDataSize, normalsData);
}

void Cloth::computeNormals() {
		
	Face *f;
	Vertex *vA, *vB, *vC;
	ofxVec3f vecCB, vecAB, faceNormal;
	
	for (int i=0; i<faces.size(); i++) {
		f = &faces[i];
		// retrieve vertices of this face;
		vA = f->vertices[0];
		vB = f->vertices[1];
		vC = f->vertices[2];
		vecAB = vB->pos - vA->pos;
		vecCB = vB->pos - vC->pos;
		//normal
		faceNormal = vecCB.getCrossed(vecAB);
		vA->n = (vA->n + faceNormal).normalize();
		vB->n = (vB->n + faceNormal).normalize();
		vC->n = (vC->n + faceNormal).normalize();
	}
}

void Cloth::update(float * distancePixels){
	
	int pos=0;
	float depthVal;
	int pW = 640.0 / (numW+1);
	int pH = 480.0 / (numH+1);
	int x, y;
	float scaleFactor = .0021;//.0021;
	float minDistance = 20;
	float prevDepth = 0;
	for(int i=0; i<numH+1; i++) {
		for(int j=0; j<numW+1; j++) {
			x = j*pW;
			y = i*pH;
			depthVal = distancePixels[j*pW + i*pH*640];
			//vertices[pos].pos.z = ofLerp(vertices[pos].pos.z, 900-(depthVal==0?200:depthVal)*6.4, 0.3);
			if (depthVal!=0) {
				//vertices[pos].pos.x = (x-640/2) * (depthVal+minDistance) * scaleFactor;
				//vertices[pos].pos.y = (y-480/2) * (depthVal+minDistance) * scaleFactor;
				//vertices[pos].pos.z = 900-(depthVal==0?200:depthVal)*6.4;
				vertices[pos].pos.x = (x-640/2) * (depthVal+minDistance) * scaleFactor;
				vertices[pos].pos.y = (y-480/2) * (depthVal+minDistance) * scaleFactor;
				vertices[pos].pos.z = -depthVal;
				prevDepth = depthVal;
			}
			else {
				vertices[pos].pos.x = (x-640/2) * (prevDepth+minDistance) * scaleFactor;
				vertices[pos].pos.y = (y-480/2) * (prevDepth+minDistance) * scaleFactor;
				vertices[pos].pos.z = -prevDepth;
				
			}
			pos++;
		}
    }
	
	// update normals
	computeNormals();
	
	// update buffer datas
	updateBufferData();
}

void Cloth::draw() {
	
	glEnable(GL_DEPTH_TEST);
	
	shader.begin();
	
	ofPushMatrix();
	glTranslatef(ofGetWidth()*0.5, ofGetHeight()*0.5, 400);
    glRotatef(rotX, 0, 1, 0);
    glRotatef(-rotY, 1, 0, 0);
	glTranslatef(-width*0.4, 100, 0);
	//glTranslatef(-width*0.5, -height*0.5, 0);
	
	setMatrixUniforms();
	
	// draw faces
		
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, false, 0, 0);	
	glVertexAttribPointer(normalAttribLoc, 3, GL_FLOAT, false, 0, (void*)(vertices.size()*3*sizeof(GLfloat)));
		
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, facesBuffer);
	glDrawElements(GL_TRIANGLES, facesSize*3, GL_UNSIGNED_INT, 0);
		
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	ofPopMatrix();
	
	shader.end();
	
	glDisable(GL_DEPTH_TEST);	
}

void Cloth::setMatrixUniforms(){
	
    glGetFloatv (GL_PROJECTION_MATRIX, pMatrix);
	glGetFloatv (GL_MODELVIEW_MATRIX, mvMatrix);
	glUniformMatrix4fv(pMatrixUniformLoc, 1,false, pMatrix);
    glUniformMatrix4fv(mvMatrixUniformLoc, 1, false, mvMatrix);
	
	ofxMatrix4x4 nMatrix;
	nMatrix=ofxMatrix4x4::getInverseOf((ofxMatrix4x4&) mvMatrix);
	nMatrix=ofxMatrix4x4::getTransposedOf((ofxMatrix4x4&) nMatrix);
	
	glUniformMatrix4fv(nMatrixUniformLoc, 1, false, nMatrix.getPtr());
}