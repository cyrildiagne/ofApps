/*
 *  Assets.h
 *  holler_logo_1
 *
 *  Created by kikko on 2/3/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#define PARTICLE "part.png"


class CachedImage : public ofImage {
	
public :
	string imgName;
};


class Assets {
	
public :
	
	static Assets * Instance();
	
	static void init();
	static ofImage* getImage(string filename);
	static ofImage* getImageByIndex(int pos); 
	
private :
	
	static void loadImg(string imgName);
	
	static Assets* _instance;
	
	vector <CachedImage *> images;
	
	static bool bInited;
	
};