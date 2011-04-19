/*
 *  LightenImage.cpp
 *  emptyExample
 *
 *  Created by Cyril Diagne on 19/04/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "LightenImage.h"

void LightenImage::setup() {

    buddahColor.loadImage("buddah.jpg");
	buddahColor.mirror(true, false);
					 
    buddahNormals.loadImage("buddah_normals.jpg");
	buddahNormals.mirror(true, false);
    
	width = buddahColor.width;
	height = buddahColor.height;
	
    shader.setup("shaders/light");
	
	setupMesh();
}

void LightenImage::setupMesh() {
	
	mesh.setMode(OF_TRIANGLE_STRIP_MODE);
	
	mesh.addVertex( ofPoint(width, height, 0) );
	mesh.addVertex( ofPoint(0, height, 0) );
	mesh.addVertex( ofPoint(width, 0, 0) );
	mesh.addVertex( ofPoint(0, 0, 0) );
	
	mesh.addNormal( ofPoint(0, 0, 1) );
	mesh.addNormal( ofPoint(0, 0, 1) );
	mesh.addNormal( ofPoint(0, 0, 1) );
	mesh.addNormal( ofPoint(0, 0, 1) );
	
	mesh.addTexCoord(ofVec2f( width, height ));
	mesh.addTexCoord(ofVec2f( 0, height ));
	mesh.addTexCoord(ofVec2f( width, 0 ));
	mesh.addTexCoord(ofVec2f( 0, 0 ));
}

void LightenImage::draw(ofCamera& camera, ofLight& light) {
	
	shader.begin();
	
	// camera matrices
	
	shader.setUniformMatrix4fv("uPMatrix", camera.getProjectionMatrix().getPtr());
	shader.setUniformMatrix4fv("uMVMatrix", camera.getModelViewMatrix().getPtr());
	
	ofMatrix4x4 nMatrix;
	nMatrix.makeInvertOf( camera.getModelViewMatrix() );
	nMatrix = ofMatrix4x4::getTransposedOf(nMatrix);
	shader.setUniformMatrix4fv("uNMatrix", nMatrix.getPtr());
	
	// light
	
	shader.setUniform3fv("uLightPos", light.getPosition().getPtr(), 3);
	
	// textures
	
	glActiveTexture(GL_TEXTURE0);
	buddahColor.bind();
	shader.setUniform1i("color", 0);
	buddahColor.unbind();
	
	glActiveTexture(GL_TEXTURE0+1);
	buddahNormals.bind();
	shader.setUniform1i("normals", 1);
	buddahNormals.unbind();
    
	
	// attributes
	
	int vPosAttLoc = shader.getAttributeLocation("aVertexPosition");
	glEnableVertexAttribArray(vPosAttLoc);
	glVertexAttribPointer(vPosAttLoc, 3, GL_FLOAT, false, 0, mesh.getVerticesPointer());
	
	int vNormalAttLoc = shader.getAttributeLocation("aVertexNormal");
	glEnableVertexAttribArray(vNormalAttLoc);
	glVertexAttribPointer(vNormalAttLoc, 3, GL_FLOAT, false, 0, mesh.getNormalsPointer());
	
	int vTexCoordsAttLoc = shader.getAttributeLocation("aTexCoords");
	glEnableVertexAttribArray(vTexCoordsAttLoc);
	glVertexAttribPointer(vTexCoordsAttLoc, 2, GL_FLOAT, false, 0, mesh.getTexCoordsPointer());
	
	// draw
	
	mesh.drawFaces();
	
	
	// clean
	
	glDisableVertexAttribArray(vPosAttLoc);
	glDisableVertexAttribArray(vNormalAttLoc);
	glDisableVertexAttribArray(vTexCoordsAttLoc);
	
	shader.end();
}