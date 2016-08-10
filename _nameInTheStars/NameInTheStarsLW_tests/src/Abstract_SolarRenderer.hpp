//
//  Abstract_SolarRenderer.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 29/04/2016.
//
//

#ifndef Abstract_SolarRenderer_hpp
#define Abstract_SolarRenderer_hpp

#include <stdio.h>
#include "OfMain.h"
#include "ofxQuadWarper.h"

class AbstractSolarRenderer {
public:
    virtual void dispose();
    virtual void setup(float width, float height) = 0;
    virtual void update( const string childName, const string fontName ) ;
    virtual void draw();
    virtual void drawDebug();
    virtual void setName(const string name);
    virtual void setFont(const string fontName);
    ofxQuadWarper quadWarper;//just make it public?
    ofTrueTypeFont font;
    const bool getDebugModeEnabled();
    virtual void setDebugModeEnabled(bool enabled);
    //need camera?
protected:
    string _childName;
    string _fontName;
    bool _debugModeEnabled;
};

#endif /* Abstract_SolarRenderer_hpp */
