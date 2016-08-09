#ifndef TJH_Globals
#define TJH_Globals
const float PRINT_DIMENSIONS_WIDTH = 3596;
const float PRINT_DIMENSIONS_HEIGHT = 2657;
const float SCREEN_SCALE_FACTOR = 0.5;
#endif

#pragma once

#include "ofMain.h"
#include "TownDashboardRenderer.hpp"
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
		
        TownDashboardRenderer renderer;
    private:
        bool townNameChanged;
        bool saveReq;
        string townName;
        string locale = "en";
        ofImage _bgImage;
        ofImage _fgImage;
        ofFbo _renderFbo;
        ofFbo _xprtFbo;
    
        int _rowIndex;
    
        std::size_t _fontIndex;
    
        ofxCsv dataCSV;
    
        bool _saveQueueEnabled;
        bool _awaitingSave;
        void nextRow();
        void prevRow();
};
