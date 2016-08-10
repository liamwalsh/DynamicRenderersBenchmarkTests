//
//  BrushWithGlowAndCirclePack.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 22/06/2016.
//
//
#pragma once
#ifndef BrushWithGlowAndCirclePack_hpp
#define BrushWithGlowAndCirclePack_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Circle.hpp"
#include "ofxFilterLibrary.h"

#endif /* BrushWithGlowAndCirclePack_hpp */

class BrushWithGlowAndCirclePack {
    public:
        BrushWithGlowAndCirclePack(ofTexture pointTexture, float density, float fboWidth, float fboHeight, float insideShapeConstraint);
    
        vector<TJH_NameInTheStarsRenderer::Circle *> circles;
        float       circleDensity;
        float       minCircleSize;
        float       maxCircleSize;
        float       circleGrowthRate;
        float       textureWhitespaceScale;
        float       textureAlpha;
    
        float       brushOutlineScatterX;
        float       brushOutlineScatterY;
        int         pathVertexSkip;
    
        ofVbo       pointPositionsVbo;
        ofFbo       alphaChannelFbo;
        ofFbo       blackBlurFbo;
        ofImage     blackPassImage;
        ofColor     baseColor;
        ofTexture   pointTexture4Shader;
        ofBlendMode blendMode;
    
        void populateFromPixels(const ofPixels &pix, ofRectangle activeBounds);
        void populateFromPixelsAndPaths(const ofPixels &pix, ofRectangle activeBounds, vector<ofPath> paths);
        //so this needs to draw from the pixels, and then the path for the outside
        vector <ofVec3f> points;
        vector <ofVec3f> sizes;
        vector <ofFloatColor> floatColors;
    
        void setGlowPass(GaussianBlurFilter *filter);
        GaussianBlurFilter *blurFilter;
        void drawWithShader(ofShader shader);
    bool glowEnabled();
    private:
    
        bool        hasGlow;
        float       _cirlceDensityMultiplier;
        float       _insideShapeConstraint;
        float       _fboWidth;
        float       _fboHeight;
        int         addCircles(int num, ofPixels pix, ofRectangle fromRect);//returns the number actually added
        void        addVertex(TJH_NameInTheStarsRenderer::Circle*);
};
