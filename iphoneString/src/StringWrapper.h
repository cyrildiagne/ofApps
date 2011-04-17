//
//  ofxString.h
//  iphoneString
//
//  Created by Cyril Diagne on 15/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

//#include "ofMain.h"
//#include "ofxiPhone.h"
//#include "ofxiPhoneExtras.h"

#import "StringOGL.h"

//@class StringOGL;

@interface StringWrapper : NSObject <StringOGLDelegate> 
{
    //
}

- (void) render;

@end
