#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(2, 1); //we define the OpenGL version we want to use
    settings.width = PRINT_DIMENSIONS_WIDTH * SCREEN_SCALE_FACTOR;
    settings.height = PRINT_DIMENSIONS_HEIGHT * SCREEN_SCALE_FACTOR;
    ofCreateWindow(settings);
    
    ofRunApp(new ofApp());

}
