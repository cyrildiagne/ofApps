/*
 *  Assets.cpp
 *  holler_logo_1
 *
 *  Created by kikko on 2/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Assets.h"

// STATIC

bool Assets::bInited = false;
Assets* Assets::_instance = 0;

Assets* Assets::Instance() {
	
	//TODO: change 0 by NULL ?
	if(_instance == 0) {
		_instance = new Assets();
		init();
	}
	return _instance;
}

void Assets::init() {
	
	if(bInited) return;
	
	loadImg(PARTICLE);
	
	bInited = true;
}

ofImage* Assets::getImage(string imgName) {
	
	for (int i=Instance()->images.size()-1; i>=0; i--) {
		if(Instance()->images[i]->imgName == imgName)
			return Instance()->images.at(i);
	}
	cout << "Assets.h : error img not found" << endl;
	return NULL;
}

ofImage* Assets::getImageByIndex(int pos) {
	
	return Instance()->images.at(pos);
}

void Assets::loadImg(string imgName) {
	
	CachedImage * img = new CachedImage();
	img->loadImage(imgName);
	img->imgName = imgName;
	Instance()->images.push_back( img );
}

// INSTANCE 
