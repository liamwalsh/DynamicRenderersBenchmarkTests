//
//  EdConcept1.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 26/05/2016.
//
//

#include "EdConcept1.hpp"
#include "ofApp.h"



void EdConcept1::setup(float width, float height)
{
    quadWarper.setSize(width, height);
    ofLog() << "Ed star 1 >> SETUP " << width << ", " << height;
    //to-do - load warp and curve settings...
    //allocation actually occurs in the update anyways.
    dotImage.allocate(128, 128, OF_IMAGE_COLOR_ALPHA);
    dotImage.load("images/stars/rate128.png");

    
}

void EdConcept1::update(const string childName, const string fontName)  {
    ofLog() << "ed stars update for name " << childName;
    font.load(fontName, 400);
    _childName = childName;
    
    //other updating?
    _drawFontFbo.clear();
    _drawFontPix.clear();
    _drawFontFbo.allocate(quadWarper.getWidth(), quadWarper.getHeight(), GL_RGB);
    _drawFontPix.allocate(quadWarper.getWidth(), quadWarper.getHeight(), OF_PIXELS_RGB);
    _drawFontFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushMatrix();
    ofTranslate(400, 1500);//THIS IS JUST A GUESS REALLY
    ofRotateY(40);
    ofRotateZ(-10);
    ofScale(1 / SCREEN_SCALE_FACTOR, 1 / SCREEN_SCALE_FACTOR);
    ofSetColor(255, 255, 255);
    font.drawString(_childName, 0, 0);
    ofPopMatrix();
    _drawFontFbo.end();
    _drawFontFbo.readToPixels(_drawFontPix);
    
    //we have drawn the font to an internal FBO
        //TODO: scaling based on text length
        //TODO: plot along a curve
        //TODO: rotate based on the curve
        //
    circles.clear();
    for (int circleStep = 0; circleStep < 4000; circleStep++)
    {
        
        for(int i = 0; i < circles.size(); i++){
            circles[i]->update();
            
            //            nameCircles[i]->update();
            //            nameCircles[i]->overlap(nameCircles);
            //            nameCircles[i]->update();
            circles[i]->overlap(circles);
        }
        
        addCircles(20);
        
    }
}

void EdConcept1::addCircles(int num)
{
    float w = quadWarper.getWidth();
    float h = quadWarper.getHeight();
    ofColor white = ofColor(255, 255, 255);
    for (int i = 0; i<num; i++)
    {
        //random point
        int x = (int)ofRandom(w);
        int y = (int)ofRandom(h);
        ofColor p = _drawFontPix.getColor(x, y);
        if( p.r > 0)
        {
            bool overlap = false;
            
            for(int j = 0; j < circles.size(); j++){
                if(ofDist(x, y, circles[j]->x, circles[j]->y) <= circles[j]->radius + 2){
                    overlap = true;
                    break;
                }
            }
            if(!overlap){
                TJH_NameInTheStarsRenderer::Circle * c = new TJH_NameInTheStarsRenderer::Circle();
                c->setup(x, y);
                circles.push_back(c);
            }
        }
    }
}

void EdConcept1::draw()   {
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    ofPushMatrix();
    //    _drawFontFbo.draw(0, 0);
    int numCircles = circles.size();
    for(int i = 0; i < numCircles; i++){
        //        nameCircles[i]->display();
        //        ofRotateZ(ofRandom(360));
        circles[i]->drawImage(dotImage);
//        circles[i]-> drawSphere();
        //        circles[i]->display();
    }
    
    ofPopMatrix();
    ofDisableBlendMode();
    
}

void EdConcept1::dispose()
{
    _drawFontFbo.clear();
    _drawFontPix.clear();
    circles.clear();
    dotImage.clear();
}
