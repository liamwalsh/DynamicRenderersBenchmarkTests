//
//  VolumetricHybridApproach.hpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 27/06/2016.
//
//

#ifndef VolumetricHybridApproach_hpp
#define VolumetricHybridApproach_hpp

#include <stdio.h>
#include "Abstract_SolarRenderer.hpp"
#include "ofxFilterLibrary.h"
#include "ofxGui.h"

#endif /* VolumetricHybridApproach_hpp */

class VolumetricHybridApproach : public AbstractSolarRenderer {
    public:
        void setup(float width, float height) override;
        void update( const string childName, const string fontName )  override;
        void draw()  override;
    
        void shaderIntValueChanged( int & intVal);
        void shaderFloatValueChanged (float & floatVal);
    
        static const ofRectangle PRINT_SCALE_RECTANGLE;
    
    private:
        ofFbo                   _textRendered;
        ofShader                _volumetricShader;
        ofImage                 _bgImgClone;
        ofRectangle             _textPositionRectangle;
        float                   _width;
        float                   _height;
        float                   _typePathScale;
        GaussianBlurFilter*      _filter1;
        GaussianBlurFilter*      _filter2;
    
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
        
        //uniforms for shader
    
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
        
    bool shaderChanged;
    
        bool hideGUI;

    
    
        void                    setupTextures();
        void                    renderName();
        void                    setupGui();
    
    
    
};