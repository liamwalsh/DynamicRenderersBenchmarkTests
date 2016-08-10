//
//  NervaticaGlowSickness.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 22/06/2016.
//
//

#include "NervaticaGlowSickness.hpp"
#include "ofApp.h"


#pragma mark Setup
void NervaticaGlowSickness::setup(float width, float height) {
    _width = width;
    _height = height;
    ofDisableArbTex();
    
    bgImgClone.load("images/bgPurple.png");
    //point shader
    
    font.load("fonts/NervaticaRegular.otf", 148 / 2, true, true, true, 0.3, 300);
//    font.load("fonts/Akzidenz Grotesk Roman Bold.ttf", 148, true, true, true, 0.3, 300);
    setupTextures();
    
    if(ofIsGLProgrammableRenderer())
    {
        texturePointsShader.load("shaders/Nervatica/150/shader");
    }
    else{
        ofLog() << " older shader loaded" ;
//        ofSetLogLevel(ofLogLevel::OF_LOG_VERBOSE);
        texturePointsShader.load("shaders/Nervatica/shader");
        //        ofSetLogLevel(ofLogLevel::OF_LOG_NOTICE);
        
    }
    setupBrushes();
}

void NervaticaGlowSickness::setupTextures()
{
    ofLoadImage(dotNoGlowTexture, "images/stars/dotBasicNoGlow.png");
    ofLoadImage(dotWithGlowTexture, "images/stars/dot.png");
    
    
}

void NervaticaGlowSickness::setupBrushes() {
    BrushWithGlowAndCirclePack baseBrush(dotWithGlowTexture, 0.5, _width, _height, 1.0);
    baseBrush.maxCircleSize = 14.0;
    baseBrush.minCircleSize = 3.0;
    baseBrush.circleDensity = 0.135;
    baseBrush.circleGrowthRate = 1.85;
    baseBrush.baseColor = ofColor(115, 156, 179);
    baseBrush.textureAlpha = 1.0;
    baseBrush.setGlowPass( new GaussianBlurFilter(_width, _height, 30.0, 1.15) );
    baseBrush.blendMode = OF_BLENDMODE_ALPHA;
    brushes.push_back(baseBrush);
    
    BrushWithGlowAndCirclePack whiteLayerSprayMid(dotWithGlowTexture, 0.65, _width, _height, 0.75);
    whiteLayerSprayMid.maxCircleSize = 9.0;
    whiteLayerSprayMid.minCircleSize = 4.0;
    whiteLayerSprayMid.circleDensity = 0.06;
    whiteLayerSprayMid.circleGrowthRate = 0.45;
    whiteLayerSprayMid.baseColor = ofColor(211, 179, 220);
    whiteLayerSprayMid.textureAlpha = 1.0;
    whiteLayerSprayMid.brushOutlineScatterY = 80.0;
    whiteLayerSprayMid.brushOutlineScatterX = 80.0;
    whiteLayerSprayMid.pathVertexSkip = 40;
////    whiteLayerS
//    //TODO :: glow
    whiteLayerSprayMid.setGlowPass( new GaussianBlurFilter(_width, _height, 40.0, 1.15) );
    brushes.push_back(whiteLayerSprayMid);
//
    BrushWithGlowAndCirclePack whiteLayerSprayFg(dotNoGlowTexture, 0.65, _width, _height, 1.0);
    whiteLayerSprayFg.maxCircleSize = 6.0;
    whiteLayerSprayFg.minCircleSize = 3.0;
    whiteLayerSprayFg.circleDensity = 0.05;
    whiteLayerSprayFg.circleGrowthRate = 0.35;
    whiteLayerSprayFg.baseColor = ofColor(255, 255, 255);
    whiteLayerSprayFg.textureAlpha = 1.0;
    
    //    whiteLayerS
    //TODO :: glow
    whiteLayerSprayFg.setGlowPass( new GaussianBlurFilter(_width, _height, 10.0, 1.35) );
    brushes.push_back(whiteLayerSprayFg);
    
    BrushWithGlowAndCirclePack bigDotNoGlow(dotWithGlowTexture, 0.5, _width, _height, 1.0);
    bigDotNoGlow.maxCircleSize = 36.0;
    bigDotNoGlow.minCircleSize = 6.0;
    bigDotNoGlow.circleDensity = 0.0025;
    bigDotNoGlow.circleGrowthRate = 0.15;
    bigDotNoGlow.baseColor = ofColor(255, 255, 255);
    bigDotNoGlow.textureAlpha = 1.0;
    bigDotNoGlow.brushOutlineScatterY = 70;
    bigDotNoGlow.brushOutlineScatterX = 70;
    bigDotNoGlow.blendMode = OF_BLENDMODE_ADD;
    bigDotNoGlow.pathVertexSkip = 90;
    //no glow
    brushes.push_back(bigDotNoGlow);
    
    BrushWithGlowAndCirclePack offPisteBigStars(dotWithGlowTexture, 0.5, _width, _height, 0.9);
    offPisteBigStars.maxCircleSize = 10.0;
    offPisteBigStars.minCircleSize = 4.0;
    offPisteBigStars.circleDensity = 0.0f;
    offPisteBigStars.circleGrowthRate = 0.10;
    offPisteBigStars.baseColor = ofColor(255, 255, 255);
    offPisteBigStars.textureAlpha = 1.0;
    offPisteBigStars.brushOutlineScatterY = 30;
    offPisteBigStars.brushOutlineScatterX = 30;
    offPisteBigStars.blendMode = OF_BLENDMODE_ADD;
    offPisteBigStars.pathVertexSkip = 400;
    //no glow
    brushes.push_back(offPisteBigStars);
    
}

//TODO :: setup the brushes - new brush type

#pragma mark update
void NervaticaGlowSickness::update(const string childName, const string fontName)
{
    _childName = ofToUpper(childName);// childName;
    renderName();
    ofPixels textPixels;
    textRendered.readToPixels(textPixels);
    auto paths = font.getStringAsPoints(_childName);//TODO - per letter
    for(auto pathItr = paths.begin(); pathItr != paths.end(); ++pathItr)
    {
        pathItr->scale(_typePathScale, _typePathScale);
    }
    //populate brushes
    
    for(auto brushItr = brushes.begin(); brushItr != brushes.end(); ++brushItr)
    {
        //TODO : pass this by reference actually
        brushItr->populateFromPixelsAndPaths(textPixels, textPositionRectangle, paths);
        brushItr->drawWithShader(texturePointsShader);
    }
    
//    for(auto brushItr = brushes.begin(); brushItr != brushes.end(); ++brushItr)
//    {
//        
//    }
    
}

void NervaticaGlowSickness::renderName()
{
    const ofRectangle PRINT_SCALE_RECTANGLE = ofRectangle(392, 547, 2773, 952);
    float scale = _width / PRINT_DIMENSIONS_WIDTH;//used for scaling rect
    textRendered.allocate(_width, _height, GL_RGBA);
    
//    font.setLetterSpacing(ofMap((float)_childName.length(), 3.0, 12.0, 1.6, 1.0));
    ofRectangle boundsOrig = font.getStringBoundingBox(_childName, 0, 0);
    ofRectangle boundsNow = ofRectangle(boundsOrig);
    ofRectangle maxBounds = ofRectangle(PRINT_SCALE_RECTANGLE);
    
    maxBounds.scale(scale);
    
    boundsNow.scaleTo(maxBounds, OF_ASPECT_RATIO_KEEP, OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER, OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);
    
    float scaleForFontNow = boundsNow.width / boundsOrig.width;
    
    _typePathScale = scaleForFontNow;
    textPositionRectangle = boundsNow;
    textPositionRectangle.setWidth(textPositionRectangle.width * 1.1);
    boundsOrig.scale(scaleForFontNow);

    textRendered.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(255, 0, 0);
//    ofDrawRectangle(maxBounds);

    ofSetColor(125, 125, 0);
//    ofDrawRectangle(boundsNow);
    
    ofPushMatrix();
    ofSetColor(255, 255, 255);
    ofTranslate(textPositionRectangle.getLeft(), textPositionRectangle.getBottom());
    ofScale(scaleForFontNow, scaleForFontNow);
    
    font.drawString(_childName, 0, 0);
    
    ofPopMatrix();
    textRendered.end();
    glDepthMask(GL_FALSE);
    ofEnablePointSprites();
    ofClear(0, 0, 0, 0);
    
}

#pragma mark draw
void NervaticaGlowSickness::draw(){
    
    glDepthMask(GL_FALSE);
    ofEnablePointSprites();
    ofClear(0, 0, 0, 0);
    bgImgClone.draw(0, 0);
    ofPushMatrix();
    ofPoint centerOfRect = textPositionRectangle.getCenter();
    ofTranslate(centerOfRect);
    ofRotate(-15);
    ofTranslate(centerOfRect.x * -1.0, centerOfRect.y * -1.0);
    for(auto brushItr = brushes.begin(); brushItr != brushes.end(); ++brushItr)
    {
        ofEnableBlendMode(brushItr->blendMode);
        brushItr->alphaChannelFbo.draw(0, 0);
        if(brushItr->glowEnabled())
        {
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            brushItr->blackBlurFbo.draw(0, 0);
        }
    }
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPopMatrix();
    ofSetColor(255, 255, 255);
    
    
    
}