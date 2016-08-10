//
//  NervaticaGlowSickness.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 22/06/2016.
//
//

#ifndef NervaticaGlowSickness_hpp
#define NervaticaGlowSickness_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Abstract_SolarRenderer.hpp"
#include "BrushWithGlowAndCirclePack.hpp"

#endif /* NervaticaGlowSickness_hpp */

class NervaticaGlowSickness : public AbstractSolarRenderer {
    public :
    
    void setup(float width, float height) override;
    void update( const string childName, const string fontName ) override;
    void draw( ) override;
    
    static const ofRectangle PRINT_SCALE_RECTANGLE;
    
    private:
    ofShader    texturePointsShader;
    ofImage     bgImgClone;
    ofRectangle textPositionRectangle;
    ofTexture   dotWithGlowTexture;
    ofTexture   dotNoGlowTexture;
    
    ofFbo       textRendered;
    vector<BrushWithGlowAndCirclePack> brushes;
    
    void setupTextures();
    void setupBrushes();
    void renderName();
    float _width;
    float _height;
    float _typePathScale;
    
};