#include "testApp.h"

#include "segment-image.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	
	kinect.init();
	kinect.setVerbose(false);
	kinect.enableDepthNearValueWhite(true);
	kinect.open();
	
	segImg.allocate(SEG_WIDTH, SEG_HEIGHT, OF_IMAGE_COLOR);
	resizeImg.allocate(640, 480, OF_IMAGE_GRAYSCALE);
	
	sigma = 0.5;
	k = 500;
	minSize = 50;
	
	input = new image<uchar>(SEG_WIDTH, SEG_HEIGHT);	
	
	gui.addTitle("Segmentation");
	gui.addSlider("sigma", sigma, 0.0, 2.0);
	gui.addSlider("k", k, 0.0, 1000.0);
	gui.addSlider("minSize", minSize, 0, 200);
	gui.show();
}

//--------------------------------------------------------------
void testApp::update(){
	
	kinect.update();
	
	if(kinect.isFrameNew()){
		
		resizeImg.setFromPixels(kinect.getDepthPixels(), 640, 480, OF_IMAGE_GRAYSCALE, true);
		resizeImg.resize(SEG_WIDTH, SEG_HEIGHT);
		
		memcpy(input->data, resizeImg.getPixels(), SEG_WIDTH * SEG_HEIGHT * sizeof(uchar));
		
		image<rgb> *rgbImage = imageGRAYtoRGB(input);
		
		image<rgb> *seg = segment_image(rgbImage, sigma, k, minSize, &numCCS); 
		segImg.setFromPixels((unsigned char *)seg->data, SEG_WIDTH, SEG_HEIGHT, OF_IMAGE_COLOR, true);
		
		delete rgbImage;
		delete seg;
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofPushMatrix();
	ofTranslate(250, 120, 0);
	
	kinect.drawDepth(0, 0, 320, 240);
	segImg.draw(320, 0);
	
	ofPopMatrix();
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

