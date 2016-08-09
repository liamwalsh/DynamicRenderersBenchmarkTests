#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void shaderParamChange();
    
    void setupGui();
    void updateText();
    void shaderIntValueChanged( int & intVal);
    void shaderFloatValueChanged (float & floatVal);
    
    
    ofFbo fbo;
    ofTexture dotTexture;
    ofImage dotImage;
    ofImage faceImage;
    
    ofTrueTypeFont font;
    ofRectangle fontBounds;
    
    ofShader shader;
    
    
    
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
    
    bool hideGUI;
    
private:
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
    bool saveRequested;
    string personName;
    ofPixels namePix;
};
