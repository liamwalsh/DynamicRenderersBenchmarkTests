//
//  NervaticaGlow_giorgia1.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 07/06/2016.
//
//

#ifndef NervaticaGlow_giorgia1_hpp
#define NervaticaGlow_giorgia1_hpp

#include <stdio.h>
#include "Abstract_SolarRenderer.hpp"
#include "BrushToVbo.hpp"
#include "ofxFilterLibrary.h"


class NervaticaGlow_giorga1 : public AbstractSolarRenderer {
    public:
    
        void setup(float width, float height) override ;
        void update( const string childName, const string fontName )  override;
        void draw()  override;
    
        ofShader texturePointsShader;
        ofShader gaussianBlurShader;
        ofShader passThruShader;
        vector<ofFbo> layerRenders;
        vector<ofImage> darkImage;
        ofImage bgImgClone;
    private:
        float _width;
        float _height;
        void setupPathMap();
        void setupBrushes();
        void drawBrushLayer(BrushToVbo brush);
        ofTexture starTexture1;
        ofFbo internalFbo;
        ofVbo tempVbo;
        //TODO :: Make a brush class
        vector<BrushToVbo> brushLayers;
        //TEMP
        
        GaussianBlurFilter *glowBlur;
        int tempBrushDebug;
//        ofxFXObject tempObj;
    
};


#endif /* NervaticaGlow_giorgia1_hpp */
