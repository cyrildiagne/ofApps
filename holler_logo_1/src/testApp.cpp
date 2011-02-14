#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	bg.loadImage("bg_dark.png");
	
	hollerLogoApp::setup();
}

void testApp::resetApp(){
	trails.clear();
}

//--------------------------------------------------------------
void testApp::updateApp(){
	
	ofPoint * pos;
	Trail *trail;
	
	bool bInsideLetter;
	
	for(int i=0; i<trails.size(); i++){
		
		trail = &trails[i];
		pos = &trail->position;
		
		bInsideLetter = false;
		
		// look if it has found a letter now
		for(int j=0; j<letters.size(); j++){
			if( j != LETTER_O_INLINE
				&& j != LETTER_E_INLINE
				&& ofInsidePoly( *pos, letters[j].points ) ) {
				
				trail->letterId = j;
				bInsideLetter = true;
				break;
			}
		}
		
		if(bInsideLetter){
			//trail->color.set(229, 69, 65, 0xff);
			trail->update(letters[trail->letterId]);
		}
		else {
			//trail->color.set(182, 49, 51, 0xAA);
			trail->update(screenBounds);
		}

		
		/*
		// If trail isn't in a letter yet
		if(trail->letterId == -1) {
			
			trail->update(screenBounds);
			
			// look if it has found a letter now
			for(int j=0; j<letters.size(); j++){
				if( ofInsidePoly( *pos, letters[j].points ) ) {
					trail->letterId = j;
					trail->color.set(0xff, 0xff, 0xff);
				}
			}
		}
		else trail->update(letters[trail->letterId]);
		*/
		
	}
}

//--------------------------------------------------------------
void testApp::drawApp(){
		
	ofSetColor(255, 255, 255, 255);
	bg.draw(0, 0);
	
	int i;
	
	if(bDebug){
		ofSetColor(90, 90, 90);
		for(i=0; i<letters.size(); i++){
			letters[i].draw();
			//letters[i].drawNormals();
		}
	}
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	for(i=0; i<trails.size(); i++){
		trails[i].draw();
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	if (prevMouse.x!=0 && prevMouse.y !=0) {
		ofPoint mouse = ofPoint(x, y);
		trails.push_back( Trail( ofPoint(x, y), mouse-prevMouse ) );
		
		if(trails.size()>MAX_TRAILS){
			trails.erase(trails.begin(), trails.begin()+(trails.size()-MAX_TRAILS));
		}
		
	}
	
	prevMouse.set(mouseX, mouseY);
}