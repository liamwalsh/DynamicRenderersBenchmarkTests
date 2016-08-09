#ifndef TJH_Globals
#define TJH_Globals
const float PRINT_DIMENSIONS_WIDTH = 3596;
const float PRINT_DIMENSIONS_HEIGHT = 2657;
const float SCREEN_SCALE_FACTOR = 0.5;
#endif

#pragma once

#include "ofMain.h"
#include "ofxCsv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    private:
        ofTrueTypeFont basicLabelFont;
        ofTrueTypeFont smallerLabelFont;
        ofRectangle maximumDoorArea;
        ofRectangle numberLabelTarget;
        ofImage     bgImage;
        ofImage     fgImage;
        ofFbo       exportFbo;
        ofxCsv      dummyDataCsv;
        string      doorLabel;
        int         csvRowIndex = 1;
        bool        updateRequired;
};
