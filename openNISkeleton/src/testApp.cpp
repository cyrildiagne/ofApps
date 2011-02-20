#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(30);
	ofBackground(90, 90, 90);
	
	//context.setupUsingRecording("SkeletonRec.oni");
	context.setupUsingXMLFile();
	image.setup(&context);
	depth.setup(&context);
	depth.getXnDepthGenerator().GetMirrorCap().SetMirror(false);
	depth.getXnDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(image.getXnImageGenerator());
	user.setup(&context, &depth);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofDisableArbTex();
	ofSetVerticalSync(true);
	
	bOrbit = bDebug = false;
	distance = 1500;
	
	cam.setFov(57.8);
	cam.setFarClip(10000.f);
	
	for(int i=0; i<kNumTestNodes; i++) {
		
		testNodes[i].setPosition(ofRandomf()*10-5, ofRandomf()*10-5, ofRandomf()*10-5);
		testNodes[i].setScale(10);
		
		// setup a traditional humanoid bind pose
		
		// LEFT ARM
		if(i==2||i==4){
			testNodes[i].bindPoseOrientation.makeRotate(-90.0, ofVec3f(0.0f, 0.0f, 1.0f));
		// RIGHT ARM
		} else if(i==3||i==5){
			testNodes[i].bindPoseOrientation.makeRotate(90.0, ofVec3f(0.0f, 0.0f, 1.0f));
		// NECK
		} else if(i==10){
			testNodes[i].bindPoseOrientation.makeRotate(180.0, ofVec3f(0.0f, 0.0f, 1.0f));
		} else {
			testNodes[i].bindPoseOrientation.set(0, 0, 0, 1);
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
	
	context.update();
	user.update();
	
	if(bOrbit) {
		cam.orbit((float)mouseX/ofGetWidth()*360, (float)mouseY/ofGetHeight()*180-90, distance, ofVec3f(0,0,distance));
	}
	else {
		cam.orbit(180, 0, distance, ofVec3f(0,0,distance));
	}
	
	if(user.getTrackedUsers().size()>0) {
		
		transformNode(0, XN_SKEL_TORSO);
		transformNode(1, XN_SKEL_WAIST);
		transformNode(2, XN_SKEL_LEFT_SHOULDER);
		transformNode(3, XN_SKEL_RIGHT_SHOULDER);
		transformNode(4, XN_SKEL_LEFT_ELBOW);
		transformNode(5, XN_SKEL_RIGHT_ELBOW);
		transformNode(6, XN_SKEL_LEFT_HIP);
		transformNode(7, XN_SKEL_RIGHT_HIP);
		transformNode(8, XN_SKEL_LEFT_KNEE);
		transformNode(9, XN_SKEL_RIGHT_KNEE);
		transformNode(10, XN_SKEL_NECK);
	}
}

//--------------------------------------------------------------
void testApp::transformNode(int nodeNum, XnSkeletonJoint skelJoint){
	
	// Adapted code from OpenNI Simbad example
	
	XnUserID userId = user.getTrackedUser(0)->id;
	
	// Get the openNI bone info	
	xn::SkeletonCapability pUserSkel = user.getXnUserGenerator().GetSkeletonCap();		
	
	XnSkeletonJointOrientation jointOri;
	pUserSkel.GetSkeletonJointOrientation(userId, skelJoint, jointOri);
	
	XnSkeletonJointPosition jointPos;
	pUserSkel.GetSkeletonJointPosition(userId, skelJoint, jointPos);
	
	if(jointOri.fConfidence > 0 )
	{
	   float * oriM = jointOri.orientation.elements;
	   
	   ofMatrix4x4 rotMatrix;
	   
	   // Create a 4x4 rotation matrix (converting row to column-major)
	   rotMatrix.set(oriM[0], oriM[3], oriM[6], 0.0f,
					 oriM[1], oriM[4], oriM[7], 0.0f,
					 oriM[2], oriM[5], oriM[8], 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
	   
	   ofQuaternion q = rotMatrix.getRotate();
	   
	   testNodes[nodeNum].setPosition(jointPos.position.X, jointPos.position.Y, jointPos.position.Z);
	   
	   // apply skeleton pose relatively to the bone bind pose
	   // /!\ WARNING the order of the quat' multiplication does mater!!
	   testNodes[nodeNum].setOrientation(testNodes[nodeNum].bindPoseOrientation*q);
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
	
	//glEnable(GL_LIGHTING);
	//ofEnableLighting();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHT0);
		
	cam.begin();
	
	if(user.getTrackedUsers().size()>0) draw3DScene();
	drawPointCloud();
	
	cam.end();
		
	ofDisableLighting();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);	
}

void testApp::keyPressed(int key){
	
	switch (key) {
		case 'o': bOrbit=!bOrbit; break;
		case 'd' : bDebug = !bDebug; break;
		case OF_KEY_UP:distance+=10; break;
		case OF_KEY_DOWN:distance-=10; break;
		default:break;
	}
	
}

void testApp::drawPointCloud() {
	
	DepthGenerator depth_generator = depth.getXnDepthGenerator();
	
	DepthMetaData dmd;
	depth_generator.GetMetaData(dmd);
	const XnDepthPixel* depth = dmd.Data();
	XN_ASSERT(depth);
	
	if (dmd.FrameID() == 0){
		return;
	}
	if (dmd.PixelFormat() == XN_PIXEL_FORMAT_RGB24) {
		printf("its in yuv\n");
	}
	
	XnUInt32 nAllIndex = (dmd.YRes() + dmd.YOffset()) * dmd.XRes(); 
	XnVector3D projective[nAllIndex]; 
	int all = 0; 
	for (XnUInt16 y = dmd.YOffset(); y < dmd.YRes() + dmd.YOffset(); y++) { 
		for (XnUInt16 x = 0; x < dmd.XRes(); x++, depth++, all++){ 
			projective[all].X = x; 
			projective[all].Y = y; 
			projective[all].Z = *depth; 
		} 
	} 
	
	XnVector3D realworld[nAllIndex]; 
	depth_generator.ConvertProjectiveToRealWorld(nAllIndex, projective, realworld); 
	
	// Draw
	
	ImageMetaData imd;
	image.getXnImageGenerator().GetMetaData(imd);
	const XnUInt8* color = imd.Data();
	XN_ASSERT(color);
	
	all = 0;
	int texture=0;
	
	glBegin(GL_POINTS);
	glColor3ub(255, 255, 255);
	for (XnUInt16 y = dmd.YOffset(); y < dmd.YRes() + dmd.YOffset(); y++) { 
		for (XnUInt16 x = 0; x < dmd.XRes(); x++, all++, texture+=3){ 
			glColor3ub(color[texture], color[texture+1], color[texture+2]);;
			glVertex3f(realworld[all].X, realworld[all].Y, realworld[all].Z); 
		} 
	}
	
	glEnd(); 
}

void testApp::draw3DScene(){
	
	//light.draw();
	
	ofDrawAxis(100);
	
	for(int i=0; i<kNumTestNodes; i++) {
		testNodes[i].draw();
	}

}

