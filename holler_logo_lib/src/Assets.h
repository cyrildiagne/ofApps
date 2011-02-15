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

#define HRED_R 227
#define HRED_G 65
#define HRED_B 75

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