//
//  BrushToVbo.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 15/06/2016.
//
//

#ifndef BrushToVbo_hpp
#define BrushToVbo_hpp

#include <stdio.h>
#include "ofMain.h"

class BrushToVbo {
    public :
    BrushToVbo(ofTexture pointTexture, float width, float height);//default constructor -> copy as we own that texture now
    ~BrushToVbo();//destructor
    float spreadX;
    float spreadY;
    float spreadZ;
    
    float insideShapeConstraint;
    int vertexSkip;
    int repeatIterations;
    
    ofBlendMode blendMode;
    
    float minSize;
    float maxSize;
    
    float size;
    ofColor baseColor;//TODO : hsv colour wheel variation?
    unsigned char hueVariance;
    float alpha;
    
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    vector <ofFloatColor> colours;
    int maxFailCount;
    void populateFromMesh(ofMesh *mesh, ofPath* path);
    
    ofTexture getTexture();
    
    ofFbo  alphaChannelFbo;
    //both of these will be blurred, and one used as a mask for the other
    ofImage  renderedOnBlackImage;
    ofImage  renderedOnBgImage;
    
    private:
    float _width;
    float _height;
    ofTexture pointTexture;
    void addPoint(float x, float y, float z, ofPath *path);
};

#endif /* BrushToVbo_hpp */
