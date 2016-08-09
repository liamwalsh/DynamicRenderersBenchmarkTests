//
//  TownDashboardRenderer.cpp
//  TJH-TownDashboard
//
//  Created by Liam Walsh on 28/06/2016.
//
//
#include "TownDashboardRenderer.hpp"
#include "ofApp.h"
#include "ofEvents.h"


TownDashboardRenderer::TownDashboardRenderer() {
    
    //allocate fbos
    _exportFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    _blackPassFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT, GL_RGB);
    _typeLayerFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    _glowPassFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    //load images
    
    blendImage.load("images/textBlendLayer.png");
    //load localised search images
    ofDirectory localesDir = ofDirectory("images/locales");
    localesDir.listDir();
    for(int i = 0; i < localesDir.size(); i++){
        auto path = localesDir.getPath(i);
        ofFile f = ofFile(path);
        if(f.isDirectory())
        {
            ofImage img = ofImage(path + "/searchingNoGlow.png");
            
            string localPath = path.substr(path.find_last_of("/") + 1, path.length());
            searchStringImages[localPath] = img;
        }
    }
    
    
    searchingStringImage = searchStringImages[_locale];
    
    backgroundImage.load("images/bgNew.png");
    
    glow.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT, GL_RGBA);
    glow.setRadius(15);
    glow.setPasses(4);
    //these are the curves
    cv.push_back(ofVec2f(2147,1297));
    cv.push_back(ofVec2f(2104,1336));
    cv.push_back(ofVec2f(2057,1374));
    cv.push_back(ofVec2f(2005,1412));
    cv.push_back(ofVec2f(1950,1446));
    cv.push_back(ofVec2f(1897,1483));
    cv.push_back(ofVec2f(1837,1517));
    cv.push_back(ofVec2f(1779,1550));
    cv.push_back(ofVec2f(1720,1580));
    cv.push_back(ofVec2f(1660,1608));
    cv.push_back(ofVec2f(1597,1635));
    cv.push_back(ofVec2f(1537,1657));
    
    spline2D.reserve(cv.size());
    for( auto p : cv)
    {
        spline2D.push_back(p);
    }
    spline2D.setInterpolation(msa::kInterpolationLinear);
    spline2D.setUseLength(false);
    
    _debugMode = false;

    font.load("Pleyo.otf", 13, true, true, true, 0.3, 300);
    font.setLetterSpacing(1.00);
    
}

void TownDashboardRenderer::draw()
{
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //type
    _typeLayerFbo.begin();
    
    ofClear(0, 0, 0, 0);
    ofBackground(0, 0, 0, 0);
    
    searchingStringImage.draw(0, 0);
    ofPushMatrix();

    ofSetColor(0,255,255);
    int maxVisibleCharacters = 14;
    string subString = _townName.length() > maxVisibleCharacters ? _townName.substr(0, maxVisibleCharacters) : _townName;
    if (_townName.length() <= (maxVisibleCharacters))
    {
        int insertWs = maxVisibleCharacters - _townName.length() - 1;
        for(int i = 0; i<insertWs; i++)
        {
            subString.push_back('p');
        }
    }
    else{
        subString += "...";
        _townName = subString;// + "...";
    }
    ofRectangle fullRect = font.getStringBoundingBox(subString, 0, 0);
    int p = 0;//running count
    ofRectangle runningStringDimensions;
    
    float debugXPos = 2600.0f;
    
    for(char &c : _townName)
    {
        int nVal = min(p + 1, (int)subString.length());
        
        string runningString = subString.substr(0, nVal);
        runningStringDimensions = font.getStringBoundingBox(runningString, 0, 0);
        string thisCharString = ofToString(c);
        ofRectangle thisCharDimensions = font.getStringBoundingBox(thisCharString, 0, 0);
        
        float diff = runningStringDimensions.width - thisCharDimensions.width;
        float debugWidth = fullRect.width;
        float dPos = diff / fullRect.width;
        ofVec2f positionOnCurve = spline2D.sampleAt(dPos);
        ofVec2f tangent = getTangent(dPos, spline2D);
        float rot = atan2(tangent.y, tangent.x) * RAD_TO_DEG;
        
        ofPushMatrix();
        
        ofTranslate(positionOnCurve.x, positionOnCurve.y);
        ofRotate(rot, 0, 0, 1);
        font.drawString(thisCharString, 0, 0);
        
        ofPopMatrix();
        
        if(_debugMode)
        {
            //debug stuff off screen
            ofSetColor(255, 0, 255);
            ofDrawRectangle(debugXPos, p * 100, runningStringDimensions.width, runningStringDimensions.height);
            ofSetColor(0, 255, 0);
            ofDrawRectangle(debugXPos + diff, runningStringDimensions.height + (p * 100), thisCharDimensions.width, 3000);
            ofSetColor(0, 0, 0);
            font.drawString(runningString, debugXPos, 50 + (p * 100));
        }
        
        ofPopMatrix();
        ++p;
        
    }
    ofSetColor(255);
    searchingStringImage.draw(0, 0);
    _typeLayerFbo.end();
    
    //drawn the type
    //black pass
    _blackPassFbo.begin();
    ofClear(0, 0, 0, 255);
    ofBackground(0, 0, 0, 255);
    
    _typeLayerFbo.draw(0, 0);
    
    _blackPassFbo.end();
    
    //glow
    glow.clear();
    glow << _blackPassFbo;
    
    
    
    _glowPassFbo.begin();
    ofClear(0, 0, 0);
    ofBackground(0, 0, 0);
    ofRectangle r = ofRectangle(0, 0, PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    glow.ofxFXObject::draw(r);
    _glowPassFbo.end();
    
    //final render
    _exportFbo.begin();
    
    ofClear(0, 0, 0);
    ofBackground(0, 0, 0);
    backgroundImage.draw(0, 0);
    blendImage.draw(0, 0);
    _typeLayerFbo.draw(0, 0);

    ofEnableBlendMode(OF_BLENDMODE_ADD);

    _glowPassFbo.draw(0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    _exportFbo.end();
    
}

ofVec2f TownDashboardRenderer::getTangent(float pos, msa::Interpolator2D &spline)
{
    static float sub = 0.01;
    float min = pos - sub >= 0 ? pos - sub: 0;
    float max = pos + sub <= 1 ? pos + sub : 1;
    ofVec2f t1 = spline.sampleAt(min);
    ofVec2f t2 = spline.sampleAt(max);
    return t2 - t1;
}

//ofVec2f TownDashboardRenderer::getTangent(float pos, ofxSimpleSpline &spline)
//{
//    ofPushStyle();
//    ofSetColor(255 * pos, 255, 0);
//    
//    static float sub = 0.01;
//    float min = pos - sub >= 0 ? pos - sub: 0;
//    float max = pos + sub <= 1 ? pos + sub : 1;
//    ofVec3f t1 = spline.getPoint(min);
//    ofVec3f t2 = spline.getPoint(max);
//    ofDrawLine(t1, t2);
//    ofPopStyle();
//    return ofVec2f(t2) - ofVec2f(t1);
//}

void TownDashboardRenderer::onTownNameChanged(string newTownName, string locale)
{
    if(locale != _locale)
    {
        _locale = locale;
        searchingStringImage = searchStringImages[_locale];
    }
    
    _townName = newTownName;
    draw();
}