#ifndef TJH_Globals
#define TJH_Globals
const float PRINT_DIMENSIONS_WIDTH = 3596;
const float PRINT_DIMENSIONS_HEIGHT = 2657;
const float SCREEN_SCALE_FACTOR = 0.5;
#endif
#pragma once
#include "ofMain.h"
#include "Abstract_SolarRenderer.hpp"
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
        int maxTestExport = 1000;
        bool exportAll = true;
        void nextLayout();
        void prevLayout();
        void nextFont();
        void prevFont();
        void prevName();
        void nextName();
        void switchRenderer();
        AbstractSolarRenderer *currentRenderer;
        std::vector<string> rendererTypes;
        ofDirectory fontsDir;
        std::map<string, AbstractSolarRenderer*> renderers;
        float _scaleFactor;
        // ^ this ? ^
        ofImage _bgImage;
        ofFbo _renderFbo;
        ofFbo _xprtFbo;
        ofImage _fgImage;
        int _nameIndex;
        int _rendererIndex;
        std::size_t _fontIndex;
        ofxCsv namesCsv;
        string _currentName;
        bool _updateRequired;
        bool _awaitingSave;
};
