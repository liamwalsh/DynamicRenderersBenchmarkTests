//
//  GiorgiaStars1.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 29/04/2016.
//
//

#include "GiorgiaStars1.hpp"
#include "ofApp.h"

void GiorgiaStars1::setup(float width, float height)
{
    quadWarper.setSize(width, height);
    ofLog() << " g star 1 setup ";
    ofDirectory svgDir = ofDirectory("svg");
//    svgDir.open("svg");
    svgDir.allowExt("svg");
    svgDir.listDir();
    ofLog() << " svg " << svgDir.size();
    auto files = svgDir.getFiles();
    auto filesBegin = files.begin();
    
    while (filesBegin != files.end())
    {
        string fName = filesBegin->getBaseName();
        ofxSVG svg;
        svg.load(svgDir.getOriginalDirectory() + filesBegin->getFileName());
        svgMap[fName] = svg;
        
        
        
        ++filesBegin;
    }
    ofLog() << " added " << svgMap.size() << " svg's ";
}

void GiorgiaStars1::draw()   {
    float xPos = 0;
    float yPos = 0;
    
    for(auto it = svgMap.begin(); it != svgMap.end(); it++)
    {
        ofxSVG shape = it->second;
        ofPushMatrix();
        ofTranslate(xPos, yPos);
        xPos += shape.getWidth();
        
        for(auto p = shape.getPaths().begin(); p != shape.getPaths().end(); p++)
        {
            ofPath n = ofPath(*p);
            n.setFilled(false);
            n.setStrokeColor(ofColor::white);
            n.hasOutline();
            n.setFillColor(ofColor::white);
            n.setStrokeWidth(3);
            ofMesh m = n.getTessellation();
            ofLog() << " " << it->first << " n col " << n.getFillColor().getHex();
//            m.addColor(ofColor::white);
            m.draw();
            
        }
//        shape.draw();
        
        ofLog() << "drawing svg " << it->first;
        if(xPos > PRINT_DIMENSIONS_WIDTH)
        {
            xPos = 0;
            yPos += shape.getHeight();
        }
        ofPopMatrix();
    }
}


void GiorgiaStars1::update(const string childName, const string fontName)  {
    ofLog() << "giorgia stars update";
}