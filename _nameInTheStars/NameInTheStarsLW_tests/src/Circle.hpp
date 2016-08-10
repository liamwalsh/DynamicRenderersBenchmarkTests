//
//  Circle.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 26/05/2016.
//
//
#pragma once
#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include "ofMain.h"
#include "RenderUtils.hpp"

#endif /* Circle_hpp */

namespace TJH_NameInTheStarsRenderer
{
    class Circle {
    public:
        Circle();
        Circle(float _minRadius, float _maxRadius, float _growRate, ofColor col);
        void setup(int _x, int _y);
        void update();
        void display();
        
        void overlap(const vector <Circle *> circles);
        void drawImage(ofTexture img);
        void drawImage(ofImage img);
        void drawSphere();
        int x;
        int y;
        
        float radius;
        float maxRadius;
        float growRate;
        float rotation;
        bool isMaxRadius;
        bool isOverlap;
        float alpha;
        ofColor c;
    };
}
