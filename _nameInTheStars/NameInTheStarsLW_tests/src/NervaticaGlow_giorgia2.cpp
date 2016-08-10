//
//  NervaticaGlow_giorgia1.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 07/06/2016.
//
//

#include "NervaticaGlow_giorgia2.hpp"
#include "Abstract_SolarRenderer.hpp"
#include "BrushToVbo.hpp"


#pragma mark setup
void NervaticaGlow_giorgia2::setup(float width, float height) {
    _width = width;
    _height = height;
    bgImgClone.load("images/bgPurple.png");
    internalFbo.allocate(width, height); //TODO: num samples
    //    ofImage starImage;
    ofDisableArbTex();
    ofLoadImage(starTexture1, "images/stars/dot.png");
    
    setupPathMap();
    setupBrushes();
    ofLog() << "loading the shader";
    
    if(ofIsGLProgrammableRenderer())
    {
        ofLog() << "should load the other damn shader";
        texturePointsShader.load("shaders/Nervatica/150/shader");
    }
    else{
        ofLog() << " load original old ass shader";
        texturePointsShader.load("shaders/Nervatica/shader");
        //        gaussianBlurShader.load("shaders/Nervatica/blur/gaussianBlur");
    }
    glowBlur = new GaussianBlurFilter(width, height, 30.f, 1.2f);
    
    ofLog() << "load passThru shader";
    passThruShader.load("shaders/Nervatica/passThru/passThru");
    //    passThruShader.linkProgram();
    
    
    ofLog() << "load obj ";
    //    tempObj.allocate(width, height, GL_RGB);
    ofSetFrameRate(1);
    
}

void NervaticaGlow_giorgia2::setupPathMap(){
    //for now let's use the svg we had previously?
    font.load("fonts/NervaticaW01Regular.ttf", 48, true, true, true, 0.3, 72);//TODO :: scale down from 400
    font.setLetterSpacing(1.1);
    ofLog() << " font loaded " << font.isLoaded();
}

void NervaticaGlow_giorgia2::setupBrushes(){
    
    ofTexture dotBasicNoGlow;
    ofLoadImage(dotBasicNoGlow, "images/stars/dotBasicNoGlow.png");
    
    ofTexture dotBasicGlow;
    ofLoadImage(dotBasicGlow, "images/stars/dot.png");
    
    BrushToVbo baseBrush(dotBasicNoGlow, _width, _height);
    baseBrush.spreadX = 50;
    baseBrush.spreadY = 50;
    baseBrush.spreadZ = 0;
    baseBrush.vertexSkip = 20;
    baseBrush.baseColor = ofColor(145, 215, 228);
    baseBrush.insideShapeConstraint = 0.0;
    baseBrush.size = 30.0;
    baseBrush.minSize = 1.0;
    baseBrush.maxSize = 50.0;
    baseBrush.repeatIterations = 3;
    baseBrush.alpha = 0.55;
    baseBrush.maxFailCount = 10000;
    baseBrush.blendMode = OF_BLENDMODE_ALPHA;
    brushLayers.push_back(baseBrush);
    
    BrushToVbo whiteSprayMidBrush(dotBasicGlow, _width, _height);
    whiteSprayMidBrush.spreadX = 15;
    whiteSprayMidBrush.spreadY = 15;
    whiteSprayMidBrush.spreadZ = 100;
    whiteSprayMidBrush.vertexSkip = 10;
    whiteSprayMidBrush.insideShapeConstraint = 0.0;
    whiteSprayMidBrush.minSize = 10.0;
    whiteSprayMidBrush.maxSize = 20.0;
    whiteSprayMidBrush.repeatIterations = 3;
    whiteSprayMidBrush.alpha = 0.7;
    whiteSprayMidBrush.baseColor = ofColor::white;//ofColor(191, 177, 200);
    whiteSprayMidBrush.blendMode = OF_BLENDMODE_ADD;
    brushLayers.push_back(whiteSprayMidBrush);
    
    BrushToVbo whiteSprayFgBrush(dotBasicNoGlow, _width, _height);
    whiteSprayFgBrush.spreadX = 10;
    whiteSprayFgBrush.spreadY = 10;
    whiteSprayFgBrush.spreadZ = 20;
    whiteSprayFgBrush.vertexSkip = 15;
    whiteSprayFgBrush.insideShapeConstraint = 0.0;
    whiteSprayFgBrush.minSize = 14.0;
    whiteSprayFgBrush.maxSize = 18.0;
    whiteSprayFgBrush.repeatIterations = 3;
    whiteSprayFgBrush.alpha = 0.85;
    whiteSprayFgBrush.baseColor = ofColor::white;//ofColor(191, 177, 200);
    whiteSprayFgBrush.blendMode = OF_BLENDMODE_ALPHA;
    brushLayers.push_back(whiteSprayFgBrush);
    
    BrushToVbo bigTightInside(dotBasicGlow, _width, _height);
    bigTightInside.spreadX = 15;
    bigTightInside.spreadY = 15;
    bigTightInside.spreadZ = 10;
    bigTightInside.vertexSkip = 750;
    bigTightInside.insideShapeConstraint = 1.0;
    bigTightInside.minSize = 32.0;
    bigTightInside.maxSize = 64.0;
    bigTightInside.repeatIterations = 1;
    bigTightInside.alpha = 0.9;
    bigTightInside.baseColor = ofColor::white;
    bigTightInside.blendMode = OF_BLENDMODE_ADD;
//    brushLayers.push_back(bigTightInside);
    ofLog() << " brush layers size " << brushLayers.size();
    
    
}

#pragma mark update
void NervaticaGlow_giorgia2::update(const string childName, const string fontName){
    
    setName(ofToUpper(childName));
    auto paths = font.getStringAsPoints(_childName);//TODO - per letter
    for(auto pathItr = paths.begin(); pathItr != paths.end(); ++pathItr)
    {
        pathItr->scale(20, 20);
    }
    //    tempVbo.clear();
    
    glDepthMask(GL_FALSE);
    //    ofDisableLighting();
    //    ofSetColor(255, 255, 255);
    
    // this makes everything look glowy :)
    
    ofEnablePointSprites();
    
    //    ofTranslate(350, 1500);
    //    ofRotate(-12, 0.0, 0.0, 1.0);
    
    //TODO: rotation?
    
    int pathCount = 0;
    int brushCount = 0;
    
    for(auto brushItr = brushLayers.begin(); brushItr != brushLayers.end(); ++brushItr)
    {
        
        brushItr->alphaChannelFbo.begin();
        ofClear(0, 0, 0, 0);
        
        //        ofBackground(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
        
        ofPushMatrix();
        ofTranslate(350, 1500);
        ofRotate(-12, 0.0, 0.0, 1.0);
        pathCount = 0;
        for(auto pathItr = paths.begin(); pathItr != paths.end(); ++pathItr)
        {
            auto mesh = pathItr->getTessellation();
//            auto mVert = mesh.getVertices();
            brushItr->populateFromMesh(&mesh, &*pathItr);
            drawBrushLayer(*brushItr);
        }
        brushCount++;
        
        ofPopMatrix();
        brushItr->alphaChannelFbo.end();
        //make fbo's for the two passes
        ofFbo tempBrushBlack;
        ofFbo tempBrushCombo;
        tempBrushBlack.allocate(_width, _height, GL_RGB);
        tempBrushCombo.allocate(_width, _height, GL_RGB);
        //then draw it on black
        //black pass
        tempBrushBlack.begin();
        ofClear(0, 0, 0, 255);
        brushItr->alphaChannelFbo.draw(0, 0);
        tempBrushBlack.end();
        ofPixels pixBlack;
        tempBrushBlack.readToPixels(pixBlack);
        brushItr->renderedOnBlackImage = ofImage(pixBlack);
        //now color
        tempBrushCombo.begin();
        ofClear(0, 0, 0);
        bgImgClone.draw(0, 0);
        brushItr->alphaChannelFbo.draw(0, 0);
        tempBrushCombo.end();
        ofPixels pixCombo;
        tempBrushCombo.readToPixels(pixCombo);
        brushItr->renderedOnBgImage = ofImage(pixCombo);
        //color pass
    }
}

void NervaticaGlow_giorgia2::drawBrushLayer(BrushToVbo brush)
{
    //blend modes are setup prior to this
    
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    vector <ofFloatColor> floatColors;
    
    copy(brush.points.begin(), brush.points.end(), back_inserter(points));
    copy(brush.sizes.begin(), brush.sizes.end(), back_inserter(sizes));
    copy(brush.colours.begin(), brush.colours.end(), back_inserter(floatColors));
    int total = (int)points.size();
    ofVbo nowThenVBO;
    nowThenVBO.clear();
    nowThenVBO.setVertexData(&points[0], total, GL_STATIC_DRAW);
    nowThenVBO.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    nowThenVBO.setColorData(&floatColors[0], total, GL_STATIC_DRAW);
    ofSetColor(255, 255, 255);
    
    texturePointsShader.begin();
    brush.getTexture().bind();
    nowThenVBO.draw(GL_POINTS, 0, total);
    brush.getTexture().unbind();
    texturePointsShader.end();
    //blend modes are disabled after this
}


#pragma mark draw
void NervaticaGlow_giorgia2::draw(){
    
    
    //draw the background
    bgImgClone.draw(0, 0);
    //then loop through the layers
    for(auto brushItr = brushLayers.begin(); brushItr != brushLayers.end(); ++brushItr)
    {
        //
        ofFbo blackMaskLayer;
        blackMaskLayer.allocate(_width, _height, GL_RGBA);
        blackMaskLayer.begin();
        ofClear(0,0,0,255);
        glowBlur->begin();
        brushItr->renderedOnBlackImage.draw(0, 0);
        glowBlur->end();
        blackMaskLayer.end();
        
        //now draw the layer
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        brushItr->alphaChannelFbo.draw(0, 0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        blackMaskLayer.draw(0, 0);
        ofDisableBlendMode();
    }
    
//    brushLayers[0].alphaChannelFbo.draw(0, 0);
//    ofFbo blackMaskLayer;
//            blackMaskLayer.allocate(_width, _height, GL_RGBA);
//            blackMaskLayer.begin();
//            ofClear(0,0,0,255);
//            glowBlur->begin();
//            brushLayers[0].renderedOnBlackImage.draw(0, 0);
//            glowBlur->end();
//    blackMaskLayer.end();
//    
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    
//    bgImgClone.draw(0, 0);
//    brushLayers[0].alphaChannelFbo.draw(0, 0);
//    blackMaskLayer.draw(0, 0);
//    
//    ofDisableBlendMode();
//    
    
    
    //    brushLayers[foo].alphaChannelFbo.draw(0, 0);
    
//    for(auto brushItr = brushLayers.begin(); brushItr != brushLayers.end(); ++brushItr)
//    {
//        ofFbo colorLayer;
//        colorLayer.allocate(_width, _height, GL_RGBA);
//        colorLayer.begin();
//        ofClear(0,0,0,255);
//        glowBlur->begin();
//        brushItr->renderedOnBgImage.draw(0, 0);
//        glowBlur->end();
//        colorLayer.end();
//        
//        ofFbo blackMaskLayer;
//        blackMaskLayer.allocate(_width, _height, GL_RGBA);
//        blackMaskLayer.begin();
//        ofClear(0,0,0,255);
//        glowBlur->begin();
//        brushItr->renderedOnBlackImage.draw(0, 0);
//        glowBlur->end();
//        blackMaskLayer.end();
//        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//        passThruShader.begin();
//        passThruShader.setUniformTexture("inputImageTexture", blackMaskLayer.getTexture(), 0);
//        passThruShader.setUniformTexture("inputImageTexture2", colorLayer.getTexture(), 1);
//        passThruShader.setUniform1f("thresholdSensitivity", 0.1);
//        passThruShader.setUniform1f("smoothing", 0.1);
//        passThruShader.setUniform3f("keyColor", 1.0, 1.0, 1.0);
//        ofDrawRectangle(0, 0, _width, _height);
//        passThruShader.end();
//        ofEnableBlendMode(brushItr->blendMode);
//        brushItr->alphaChannelFbo.draw(0, 0);
//        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//        
//    }
    //
    //    ofFbo colorLayer;
    //    colorLayer.allocate(_width, _height, GL_RGBA);
    //    colorLayer.begin();
    //    ofClear(0,0,0,255);
    //    glowBlur->begin();
    //    brushLayers[1].renderedOnBgImage.draw(0, 0);
    //    glowBlur->end();
    //
    //    colorLayer.end();
    //
    //    ofFbo blackMaskLayer;
    //    blackMaskLayer.allocate(_width, _height, GL_RGBA);
    //    blackMaskLayer.begin();
    //    ofClear(0,0,0,255);
    //    glowBlur->begin();
    //    brushLayers[1].renderedOnBlackImage.draw(0, 0);
    //    glowBlur->end();
    //    blackMaskLayer.end();
    //
    //    passThruShader.begin();
    //    passThruShader.setUniformTexture("inputImageTexture", blackMaskLayer.getTexture(), 0);
    //    passThruShader.setUniformTexture("inputImageTexture2", colorLayer.getTexture(), 1);
    //    passThruShader.setUniform1f("thresholdSensitivity", 0.1);
    //    passThruShader.setUniform1f("smoothing", 0.1);
    //    passThruShader.setUniform3f("keyColor", 1.0, 1.0, 1.0);
    //    ofDrawRectangle(0, 0, _width, _height);
    //    passThruShader.end();
    //    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //    brushLayers[1].alphaChannelFbo.draw(0, 0);
    //    ofDisableBlendMode();
}


//void NervaticaGlow_giorga1::draw(){
//
//    //temp test to see if we drew the brush layers correctly
//
//    //    brushLayers[foo].alphaChannelFbo.draw(0, 0);
//
//    ofFbo colorLayer;
//    colorLayer.allocate(_width, _height, GL_RGBA);
//    colorLayer.begin();
//    ofClear(0,0,0,255);
//    glowBlur->begin();
//    brushLayers[1].renderedOnBgImage.draw(0, 0);
//    glowBlur->end();
//
//    colorLayer.end();
//
//    ofFbo blackMaskLayer;
//    blackMaskLayer.allocate(_width, _height, GL_RGBA);
//    blackMaskLayer.begin();
//    ofClear(0,0,0,255);
//    glowBlur->begin();
//    brushLayers[1].renderedOnBlackImage.draw(0, 0);
//    glowBlur->end();
//    blackMaskLayer.end();
//
//    passThruShader.begin();
//    passThruShader.setUniformTexture("inputImageTexture", blackMaskLayer.getTexture(), 0);
//    passThruShader.setUniformTexture("inputImageTexture2", colorLayer.getTexture(), 1);
//    passThruShader.setUniform1f("thresholdSensitivity", 0.1);
//    passThruShader.setUniform1f("smoothing", 0.1);
//    passThruShader.setUniform3f("keyColor", 1.0, 1.0, 1.0);
//    ofDrawRectangle(0, 0, _width, _height);
//    passThruShader.end();
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    brushLayers[1].alphaChannelFbo.draw(0, 0);
//    ofDisableBlendMode();
//}
