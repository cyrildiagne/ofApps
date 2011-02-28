#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	ofBackground(90, 90, 90);
	
	//context.setupUsingRecording("SkeletonRec.oni");
#ifdef PLAYBACK
	context.setupUsingRecording(ofToDataPath("record.oni"));
#else
	context.setup();
	context.setupUsingXMLFile();
#endif
	
	depth.setup(&context);
	image.setup(&context);
	user.setup(&context, &depth, &image);
	depth.toggleRegisterViewport(&image);
	context.toggleMirror();

#ifndef PLAYBACK
	recorder.setup(&context, &depth, &image);
#endif
	
	userPointCloud.init(depth, image, user);
	bgPointCloud.init(depth, image, user, POINTCLOUD_SCENE);
	scenePointCloud.init(depth, image, user, POINTCLOUD_SCENE);
	
	ofSetLogLevel(OF_LOG_WARNING);
	ofDisableArbTex();
	ofSetVerticalSync(true);
	
	bOrbit = bDebug = bRecord = false;
	bRecordBackground = true;
	
	camFOV = 68.3;
	distance = 2500;
	
	camera.setFov(camFOV);
	camera.setFarClip(10000.f);
}

//--------------------------------------------------------------
void testApp::update(){
	
	context.update();
	user.update();
	
	if(bRecordBackground) {
		bgPointCloud.update();
		bRecordBackground = false;
	}
	
	if(user.getTrackedUsers().size()==0) {
		scenePointCloud.update();
	} else {
		userPointCloud.update();
	}
	
	if(bOrbit) {
		camera.orbit((float)mouseX/ofGetWidth()*360, (float)mouseY/ofGetHeight()*180-90, distance, ofVec3f(0,0,distance));
	}
	else {
		camera.orbit(180, 0, distance, ofVec3f(0,0,distance));
	}
}


//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255, 255, 255);
	
	if (bDebug) {
		depth.draw(0,0,320,240);
		image.draw(320, 0, 320, 240);
	}
	//user.draw();
	
	// setup scene Rendering
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
		
	camera.begin();
	
	if(user.getTrackedUsers().size()==0) {
		scenePointCloud.draw();
	} else {
		bgPointCloud.draw();
		userPointCloud.draw();
	}
	
	camera.end();
		
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	if (bRecord) {
		ofSetColor(255, 0, 0);
		ofCircle(15, 15, 10);
		ofSetColor(255, 255, 255);
	}
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 2), 5, 5);
}

void testApp::keyPressed(int key){
	
	switch (key) {
		case ' ': bRecordBackground = true; break;
		case 'o': bOrbit=!bOrbit; break;
		case 'd': bDebug=!bDebug; break;
		case OF_KEY_UP: distance+=10; break;
		case OF_KEY_DOWN: distance-=10; break;
#ifndef PLAYBACK
		case 'r': bRecord=!bRecord; 
			if(bRecord) {
				recorder.startRecord("record.oni");
			}	
			break;
#endif
		default : break;
	}
}