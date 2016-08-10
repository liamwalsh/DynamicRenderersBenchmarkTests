//
//  EdConcept1.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 26/05/2016.
//
//

#ifndef EdConcept1_hpp
#define EdConcept1_hpp

#include <stdio.h>
#include "Abstract_SolarRenderer.hpp"
#include "RenderUtils.hpp"
#include "Circle.hpp"

class EdConcept1 : public AbstractSolarRenderer {
    public:
        void setup(float width, float height) override ;
        void update( const string childName, const string fontName )  override;
        void draw()  override;
        void dispose() override;
    private:
        ofFbo _drawFontFbo;
        ofPixels _drawFontPix;
        vector <TJH_NameInTheStarsRenderer::Circle *> circles;
        void addCircles(int num);
        ofImage dotImage;
    
};

#endif /* EdConcept1_hpp */