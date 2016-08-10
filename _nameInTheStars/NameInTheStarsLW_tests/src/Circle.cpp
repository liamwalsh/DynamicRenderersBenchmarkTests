//
//  Circle.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 26/05/2016.
//
//

#include "Circle.hpp"
TJH_NameInTheStarsRenderer::Circle::Circle(float _minRadius, float _maxRadius, float _growRate, ofColor col)
{
    alpha = ofRandom(200, 255);
    radius = _minRadius;
    growRate = _growRate;
    maxRadius = _maxRadius;
    rotation = ofRandom(360);
    c = col;
}

TJH_NameInTheStarsRenderer::Circle::Circle(){
    rotation = ofRandom(360);
    radius = 0;
    maxRadius = ofRandom(8, 20);
    growRate = ofRandom(1, 2);
    c.set(255, 255, ofRandom(160, 255));
}

//--------------------------------------------------------------
void TJH_NameInTheStarsRenderer::Circle::setup(int _x, int _y){
    x = _x;
    y = _y;
    
    
    isMaxRadius = false;
    isOverlap = false;
    
    
    
}

//--------------------------------------------------------------
void TJH_NameInTheStarsRenderer::Circle::update(){
    if(!isMaxRadius && !isOverlap){
        radius += growRate;
        isMaxRadius = radius >= maxRadius;
    }
}

//--------------------------------------------------------------
void TJH_NameInTheStarsRenderer::Circle::display(){
    ofFill();
    ofSetColor(c);
    ofDrawCircle(x, y, radius);
//    ofNoFill();
//    ofSetColor(0);
//    ofDrawCircle(x, y, radius);
}

void TJH_NameInTheStarsRenderer::Circle::overlap(const vector <Circle *> circles){
    if(!isMaxRadius && !isOverlap){
        for(int i = 0; i < circles.size(); i++){
            if(circles[i] != this){
                if(ofDist(x, y, circles[i]->x, circles[i]->y) <= circles[i]->radius + radius + growRate){
                    isOverlap = true;
                }
            }
        }
    }
}
void TJH_NameInTheStarsRenderer::Circle::drawImage(ofTexture img)
{
    
    img.draw(x, y, radius * 2, radius * 2);
}

void TJH_NameInTheStarsRenderer::Circle::drawImage(ofImage img)
{
    ofPushMatrix();
    ofTranslate(x, y, 1);
    ofRotateZ(rotation);
//    c.a = 200 + (55 * (radius / 15));
    ofSetColor(c);
    img.draw(-radius, -radius, radius * 2, radius * 2);
    ofPopMatrix();
}

void TJH_NameInTheStarsRenderer::Circle::drawSphere()
{
    ofFill();
    ofSetColor(c);
    ofDrawSphere(x, y,radius);
    
}
