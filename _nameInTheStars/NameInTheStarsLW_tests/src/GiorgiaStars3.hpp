//
//  GiorgiaStars3.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 02/06/2016.
//
//

#ifndef GiorgiaStars3_hpp
#define GiorgiaStars3_hpp

#include <stdio.h>
#include "OfMain.h"
#include "Abstract_SolarRenderer.hpp"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "Circle.hpp"

#endif /* GiorgiaStars3_hpp */

class GiorgiaStars3 : public AbstractSolarRenderer {
    public:
        void setup(float width, float height) override ;
        void update( const string childName, const string fontName )  override;
        void draw()  override;
        void dispose() override;
    
    private:
        ofFbo _drawFontFbo;
        ofxCvContourFinder 	contourFinder;
        ofPixels _drawFontPix;
        vector <TJH_NameInTheStarsRenderer::Circle *> circles;
        int addCircles(int num);
};