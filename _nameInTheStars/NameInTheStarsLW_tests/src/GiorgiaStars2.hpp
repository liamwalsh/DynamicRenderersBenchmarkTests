//
//  GiorgiaStars2.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 24/05/2016.
//
//

#ifndef GiorgiaStars2_hpp
#define GiorgiaStars2_hpp

#include <stdio.h>
#include "OfMain.h"
#include "Abstract_SolarRenderer.hpp"
#include "ofxOpenCv.h"
#include "ofxGui.h"

#endif /* GiorgiaStars2_hpp */
class GiorgiaStars2 : public AbstractSolarRenderer {
    public:
        void setup(float width, float height) override ;
        void update(const string childName, const string fontName)  override;
        void draw()  override;
    
        void shaderIntValueChanged( int & intVal);
        void shaderFloatValueChanged (float & floatVal);
        void drawDebug() override;
    private:
        void setupGui();
        ofFbo _drawFontFbo;
        ofxCvContourFinder 	contourFinder;
        ofPixels sdfPixels;
        ofImage sdfImage;
        //shader stuff
        ofShader shader;
        bool shaderChanged;
        //to-do more shader type things
        ofxPanel gui;
        
        ofxIntSlider iterationSlider;
        ofxIntSlider iterationsMultiplierSlider;
        
        ofxIntSlider volSteps;
        ofxIntSlider volStepsMultiplier;
        
        ofxIntSlider timeSlider;
        ofxIntSlider timeMultiplierSlider;
        
        ofxFloatSlider forumparamSlider;
        ofxFloatSlider forumparamMultiplierSlider;
        
        ofxFloatSlider stepsizeSlider;
        ofxFloatSlider stepsizeMultiplierSlider;
        
        ofxFloatSlider zoomSlider;
        ofxFloatSlider zoomMultiplierSlider;
        
        ofxFloatSlider brightnessSlider;
        ofxFloatSlider brightnessMultiplierSlider;
        
        ofxFloatSlider darkmatterSlider;
        ofxFloatSlider distFadingSlider;
        ofxFloatSlider distFadingMultiplierSlider;
        ofxFloatSlider saturationSlider;
        ofxFloatSlider saturationMultiplierSlider;
        
        ofxFloatSlider tileSlider;
        
        ofParameter<int> _iterations;
        ofParameter<int> _iterationMultiplier;
        
        ofParameter<int> _volSteps;
        ofParameter<int> _volStepsMultiplier;
        
        ofParameter<float> _forumparam;
        ofParameter<float> _forumparamMultiplier;
        
        ofParameter<float> _stepsize;
        ofParameter<float> _stepsizeMultiplier;
        
        ofParameter<float> _zoom;
        ofParameter<float> _zoomMultiplier;
        
        ofParameter<float> _brightness;
        ofParameter<float> _brightnessMultiplier;
        
        ofParameter<float> _darkmatter;
        
        ofParameter<float> _distfading;
        ofParameter<float> _distfadingMultiplier;
        
        ofParameter<float> _saturation;
        ofParameter<float> _saturationMultiplier;
        
        ofParameter<float> _tile;
        ofParameter<int> _time;
    
};