#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	
	kinect.init();
	kinect.setVerbose(true);
	kinect.enableDepthNearValueWhite(true);
	kinect.open();
	
	bSetup = true;
	
	depthImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
	rgbaImg.allocate(kinect.width, kinect.height, 0);
	
	cloth.init(kinect.width, kinect.height, 128, 96);
	//kinectNormal.init(depthImage);
	
	int length = kinect.width*kinect.height*4;
	rgbaPixels = new unsigned char[length];
	memset(rgbaPixels, 0, length*sizeof(unsigned char));
	
	nearThreshold = 255;
	farThreshold  = 0;
	
	ofBackground(120, 120, 120);
	
	ofSetVerticalSync(false);
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	
	ofDisableArbTex();
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
}

//--------------------------------------------------------------
void testApp::update()
{
	kinect.update();
			
	if(kinect.isFrameNew()) {
		
		// threshold depth image
		unsigned char * depthPixels = kinect.getDepthPixels();
		int numPixels = depthImage.getWidth() * depthImage.getHeight();
		for(int i = 0; i < numPixels; i++){
			if( depthPixels[i] > nearThreshold || depthPixels[i] < farThreshold ){
				depthPixels[i] = 0;
			}
		}
		depthImage.setFromPixels(depthPixels, kinect.width, kinect.height, OF_IMAGE_GRAYSCALE, true);
		
		// put depth info in alpha channel
		unsigned char * kinectColorPixels = kinect.getPixels();
		unsigned char * rgbaP = rgbaPixels;
		for(int i = 0; i < numPixels; i++){
			*rgbaP++ = kinectColorPixels[i*3];
			*rgbaP++ = kinectColorPixels[i*3+1];
			*rgbaP++ = kinectColorPixels[i*3+2];
			*rgbaP++ = depthPixels[i];
		}
		rgbaImg.setFromPixels(rgbaPixels, kinect.width, kinect.height, OF_IMAGE_COLOR_ALPHA, true);
		
		//kinectNormal.update(depthImage);
		cloth.update(kinect.getDistancePixels());
	}
	
	cloth.rotX = ofLerp(cloth.rotX, (mouseX-ofGetWidth()*0.5)*0.1, 0.1);
    cloth.rotY = ofLerp(cloth.rotY, (mouseY-ofGetHeight()*0.5)*0.1, 0.1);
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetHexColor(0xffffff);
	
	if(bSetup){
		
		ofPushMatrix();
		ofTranslate(200, -100, 0);
	}
	cloth.draw();
	
	if(bSetup) {
		
		ofPopMatrix();
		
		ofEnableAlphaBlending();
		kinect.drawDepth(10, 10, 200, 150);
		kinect.draw(220, 10, 200, 150);
		depthImage.draw(10, 170, 200, 150);
		ofSetHexColor(0);
		ofRect(220, 170, 200, 150);
		ofSetHexColor(0xffffff);
		rgbaImg.draw(220, 170, 200, 150);
		//kinectNormal.draw(440, 10, 200, 150);
				
		char reportStr[1024];
		sprintf(reportStr, "nearThreshold %i (press: + -)\nfarThreshold %i (press: < >)", nearThreshold, farThreshold);
		ofDrawBitmapString(reportStr, 20, 340);
		ofDisableAlphaBlending();
	}
	
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1)+"fps", ofGetWidth()-60, 13);
}

//--------------------------------------------------------------
void testApp::exit(){
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key)
{
	switch (key)
	{	
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
		case '<':		
		case ',':		
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
		case '-':		
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 's':
			bSetup=!bSetup;
			break;
		case 'f':
			ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{}

