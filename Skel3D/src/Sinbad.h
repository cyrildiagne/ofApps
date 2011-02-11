/*
 *  Sinbad.h
 *  Skel3D
 *
 *  Created by kikko on 1/21/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SINBAD
#define _SINBAD

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Sinbad : public ofMeshNode {

public:
	
	Sinbad() {
	}
	
	void load() {
		model.loadModel("astroboy.dae");
		listBones();
	}
	
	void listBones() {
		
		// update mesh position for the animation
		for (unsigned int i = 0; i < model.modelMeshes.size(); ++i){
			
			// current mesh we are introspecting
			const aiMesh* mesh = model.modelMeshes[i].mesh;
			
			
			cout << "--- bones list ---" << endl;
			
			// calculate bone matrices
			std::vector<aiMatrix4x4> boneMatrices( mesh->mNumBones);
			for( size_t a = 0; a < mesh->mNumBones; ++a)
			{
				const aiBone* bone = mesh->mBones[a];
				cout << bone->mName.data << endl;
			}
			
			cout << "-----------------" << endl;
		}
	}
	
	aiNode* void getBoneNode(string boneName) {
		return model.scene->mRootNode->FindNode(boneName);
	}
	
	void customDraw() {
		model.draw();	
	}

private:
	
	ofxAssimpModelLoader model;
	
};

#endif