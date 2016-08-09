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
        void setupLoadImages();
        void setupTypefaces();
        void setupGeometry();
        ofVec2f getTangent(float pos, ofPolyline curve);
        void updateAddressChange(string doorNameOrNumber, string streetName, string townName, string cityName, string countryName);
        void updateAddressChange(string doorNameOrNumber, string streetName, string townName, string cityName); //no country
        void drawPoster();
        void drawRing(int index);
        void drawName();
        float getArcAngleForLength(float stringLength, float radius);
        ofFbo posterFbo;
        ofFbo exportFbo;
        ofImage backgroundImage;
        ofImage foregroundImage;
        map<string, ofImage> foregroundImages;
        vector<string>addressComponents;
        vector<ofTrueTypeFont> rainbowFonts;
        vector<ofTrueTypeFont> nameFonts;//this might change I guess
    
        vector<ofColor>textColors;
        vector<ofPolyline> arcs;
        vector<float>rainbowRingRadii;
        vector<float>nameRingRadii;
        vector<ofImage>rainbows;
        map<string, string>countryLookUp;
        int csvRowIndex;
        ofxCsv dummyDataCsv;
        bool updateRequired;
        ofPoint circleCenterPoint;
    
        void csvNextRow();
        void csvPrevRow();
    
};
