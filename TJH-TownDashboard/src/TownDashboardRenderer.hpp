//
//  TownDashboardRenderer.hpp
//  TJH-TownDashboard
//
//  Created by Liam Walsh on 28/06/2016.
//
//

#pragma once
#include <stdio.h>
#include "ofMain.h"
#include "ofxGlow.h"
#include "ofxGaussianBlur.h"
#include "MSAInterpolator.h"


class TownDashboardRenderer {
    public:
        TownDashboardRenderer();//default constructor
        ofFbo               _exportFbo;
        ofFbo               _typeLayerFbo;//draw onto this
        ofFbo               _blackPassFbo;
        ofFbo               _glowPassFbo;
        void draw();
        void onTownNameChanged(string newTownName, string locale = "en");
        void clickMouse(ofMouseEventArgs & args);
        ofxGaussianBlur             glow;
//        GaussianBlurFilter      *blurFilter;
    
    private:
        msa::Interpolator2D      spline2D;
        vector<ofVec2f>     cv;
        ofVec3f             pointOnCurve;
        ofTrueTypeFont      font;
        float               _width;
        float               _height;
        map<string, ofImage> searchStringImages;
        ofPoint             _fboDrawOffset;
        ofEasyCam           cam;
        bool                _debugMode;
        string              _townName;
        string              _locale = "fr";
//        ofVec2f             getTangent(float pos, ofxSimpleSpline& spline);
        ofVec2f             getTangent(float pos, msa::Interpolator2D &spline);
        ofImage             blendImage;
        ofImage             searchingStringImage;
        ofImage             backgroundImage;//need to blend against the background
    
};