#include "ofMain.h"
#include "ofApp.h"




//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(2, 1); //we define the OpenGL version we want to use
    settings.width = PRINT_DIMENSIONS_WIDTH * SCREEN_SCALE_FACTOR;
    settings.height = PRINT_DIMENSIONS_HEIGHT * SCREEN_SCALE_FACTOR;
    
    ofCreateWindow(settings);
//	ofSetupOpenGL(PRINT_DIMENSIONS_WIDTH * SCREEN_SCALE_FACTOR, PRINT_DIMENSIONS_HEIGHT * SCREEN_SCALE_FACTOR ,OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    
	ofRunApp(new ofApp());

}