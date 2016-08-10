//
//  NervaticaGlow_giorgia2.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 22/06/2016.
//
//

#ifndef NervaticaGlow_giorgia2_hpp
#define NervaticaGlow_giorgia2_hpp

#include <stdio.h>
#include "Abstract_SolarRenderer.hpp"
#include "BrushToVbo.hpp"
#include "ofxFilterLibrary.h"

#endif /* NervaticaGlow_giorgia2_hpp */
class NervaticaGlow_giorgia2 : public AbstractSolarRenderer {
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