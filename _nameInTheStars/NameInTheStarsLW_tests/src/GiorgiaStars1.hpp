//
//  GiorgiaStars1.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 29/04/2016.
//
//

#ifndef GiorgiaStars1_hpp
#define GiorgiaStars1_hpp

#include <stdio.h>
#include "OfMain.h"
#include "Abstract_SolarRenderer.hpp"
#include "ofxSvg.h"

class GiorgiaStars1 : public AbstractSolarRenderer {
    
    void setup(float width, float height) override ;
    void update( const string childName, const string fontName )  override;
    void draw()  override;
    
    unordered_map<string, ofxSVG> svgMap;
    
};

#endif /* GiorgiaStars1_hpp */
