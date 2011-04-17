#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	ofBackground(90, 90, 90);
	
#ifdef PLAYBACK
    oni.initWithRecording(ofToDataPath("record.oni"));
#else
    oni.initWithXML();
    oni.depth.getXnDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(oni.image.getXnImageGenerator());
    recorder.setup(&oni.context, &oni.depth, &oni.image);
#endif
	
	userPointCloud.init(oni);
	bgPointCloud.init(oni, POINTCLOUD_SCENE);
	scenePointCloud.init(oni, POINTCLOUD_SCENE);
	
#ifdef EXPORT_VIDEO
	screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
	videoSaver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_HIGH);
	videoSaver.setup(ofGetWidth(), ofGetHeight(), "test_Export.mov");
#endif
    
	ofSetLogLevel(OF_LOG_WARNING);
	//ofDisableArbTex();
	ofSetVerticalSync(true);
	
    bDrawSkinnedUserPointCloud = false;
    
	bOrbit = bRecord = false;
	bRecordBackground = bDebug = true;
	
	camFOV = 68.3;
	distance = 2500;
	
	camera.setFov(camFOV);
	camera.setFarClip(10000.f);
}

void testApp::exit() {
#ifdef EXPORT_VIDEO
	videoSaver.finishMovie();
#endif
}

//--------------------------------------------------------------
void testApp::update(){
	
	oni.update();
	
	 if(oni.user.getTrackedUsers().size() > 0) {
         
        if(bRecordBackground) {
            bgPointCloud.update();
            bRecordBackground = false;
        }
         
		userPointCloud.update();
         
        if(!bDrawSkinnedUserPointCloud) scenePointCloud.update();
         
	} else {
        
        scenePointCloud.update();
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
		oni.depth.draw(0,0,320,240);
		oni.image.draw(320, 0, 320, 240);
	}
	//user.draw();
	
	// setup scene Rendering
	
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
		
	camera.begin();
	
	if(oni.user.getTrackedUsers().size()>0) {
        
        if (bDrawSkinnedUserPointCloud) {
            bgPointCloud.draw();
            userPointCloud.draw();
        }
        else {
            scenePointCloud.draw();
        }

        if(bDebug) userPointCloud.drawBones();
    
	} else {
        
		scenePointCloud.draw();
	}
	
	camera.end();
		
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	
	if (bRecord) {
		ofSetColor(255, 0, 0);
		ofCircle(15, 15, 10);
		ofSetColor(255, 255, 255);
	}
#ifdef EXPORT_VIDEO
	screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	videoSaver.addFrame(screen.getPixels(), 1.0 / 28.0);
#endif
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