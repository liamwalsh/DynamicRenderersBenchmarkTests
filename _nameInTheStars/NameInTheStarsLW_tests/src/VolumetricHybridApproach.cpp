//
//  VolumetricHybridApproach.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 27/06/2016.
//
//

#include "VolumetricHybridApproach.hpp"
#include "ofApp.h"
#include "ofxFilterLibrary.h"


void VolumetricHybridApproach::setup(float width, float height)
{
    _width = width;
    _height = height;
    //
    _bgImgClone.load("images/bgPurple.png");
    
    _textRendered.allocate(_width, _height);
    _filter1 = new GaussianBlurFilter(_width, _height, 90.0, 1.0);
    _filter2 = new GaussianBlurFilter(_width, _height, 40.0, 1.1);
    ofSetLogLevel(ofLogLevel::OF_LOG_VERBOSE);
    if(ofIsGLProgrammableRenderer())
    {
        ofLog() << "actually is programmable";
    }
    else{
        ofLog() << " not programmable";
    }
    string shaderProgram = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect tex0;\
    \
    void main (void){\
    vec2 pos = gl_TexCoord[0].st;\
    \
    vec3 src = texture2DRect(tex0, pos).rgb;\
    float mask = 1.0;\
    \
    gl_FragColor = vec4( src , mask);\
    }";
    _volumetricShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    _volumetricShader.linkProgram();
//    _volumetricShader.load("shaders/Volumetric/volumetricStars");
////    _volumetricShader.setupShaderFromFile(GL_FRAGMENT_SHADER, "shaders/Volumetric/volumetricStars.frag");
//    _volumetricShader.bindDefaults();
    _volumetricShader.linkProgram();
//    _volumetricShader.load("shaders/Volumetric/volumetricStars");
    //TODO :: gotta clean up all these dangling pointers bro
    //font
    shaderChanged = true;
    
}

void VolumetricHybridApproach::shaderIntValueChanged(int &intVal)
{
    shaderChanged = true;
}

void VolumetricHybridApproach::shaderFloatValueChanged(float &floatVal)
{
    shaderChanged = true;
}
void VolumetricHybridApproach::setupGui()
{
    gui.setup("solarPanel");
    gui.setSize(500, 500);
    gui.setDefaultWidth(500);
    
    _iterations.set("iterations (number of stars)", 17);
    _iterations.setMin(0);
    _iterations.setMax(30);
    gui.add(iterationSlider.setup(_iterations));
    iterationSlider.addListener(this, &VolumetricHybridApproach::shaderIntValueChanged);
    
    _iterationMultiplier.set("iteration Face Multiplier", 0);
    _iterationMultiplier.setMin(0);
    _iterationMultiplier.setMax(15);
    
    gui.add(iterationsMultiplierSlider.setup(_iterationMultiplier));
    iterationsMultiplierSlider.addListener(this, &VolumetricHybridApproach::shaderIntValueChanged);
    
    _volSteps.set("volume Steps (galaxy-ness) ", 30);
    _volSteps.setMax(100);
    _volSteps.setMin(0);
    
    gui.add(volSteps.setup(_volSteps));
    volSteps.addListener(this, &VolumetricHybridApproach::shaderIntValueChanged);
    
    _volStepsMultiplier.set("volume steps Face multiplier", 0);
    _volStepsMultiplier.setMin(0);
    _volStepsMultiplier.setMax(100);
    gui.add(volStepsMultiplier.setup(_volStepsMultiplier));
    volStepsMultiplier.addListener(this, &VolumetricHybridApproach::shaderIntValueChanged);
    
    _forumparam.set("stary-ness (speckle)", 0.53);
    _forumparam.setMin(0.45);
    _forumparam.setMax(1.0);
    gui.add(forumparamSlider.setup(_forumparam));
    forumparamSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _forumparamMultiplier.set("speckle name multiplier", 0);
    _forumparamMultiplier.setMin(0);
    _forumparamMultiplier.setMax(0.1);
    gui.add(forumparamMultiplierSlider.setup(_forumparamMultiplier));
    _forumparamMultiplier.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _stepsize.set("step size (density)", 0.17);
    _stepsize.setMax(0.9);
    _stepsize.setMin(0.00);
    gui.add(stepsizeSlider.setup(_stepsize));
    stepsizeSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _stepsizeMultiplier.set("step size name multiplier", 0);
    _stepsizeMultiplier.setMax(0.2);
    _stepsizeMultiplier.setMin(0);
    gui.add(stepsizeMultiplierSlider.setup(_stepsizeMultiplier));
    stepsizeMultiplierSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _zoom.set("zoom (camera distance)", 0.8);
    _zoom.setMin(0.05);
    _zoom.setMax(100.0);
    gui.add(zoomSlider.setup(_zoom));
    zoomSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    //brightness, darkmatter, distfading, saturation, tile
    _brightness.set("brightness", 0.0015);
    _brightness.setMax(0.0085);
    _brightness.setMin(0.0005);
    gui.add(brightnessSlider.setup(_brightness));
    brightnessSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _darkmatter.set("darkmatter (steals light)", 0.3);
    _darkmatter.setMax(0.99);
    _darkmatter.setMin(0.1);
    gui.add(darkmatterSlider.setup(_darkmatter));
    darkmatterSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _distfading.set("dist fading (more = bluer & brighter)", 0.73);
    _distfading.setMin(0.5); _distfading.setMax(0.95);
    gui.add(distFadingSlider.setup(_distfading));
    distFadingSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _distfadingMultiplier.set("dist fading name multiplier", 0);
    _distfadingMultiplier.setMin(0.0);
    _distfadingMultiplier.setMax(1.0);
    gui.add(distFadingMultiplierSlider.setup(_distfadingMultiplier));
    distFadingMultiplierSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    
    _saturation.set("saturation", 0.8);
    _saturation.setMin(0.5);
    _saturation.setMax(0.99);
    gui.add(saturationSlider.setup(_saturation));
    saturationSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _tile.set("tile ratio (wrap)", 0.85);
    _tile.setMin(0.5);
    _tile.setMax(3.0);
    gui.add(tileSlider.setup(_tile));
    tileSlider.addListener(this, &VolumetricHybridApproach::shaderFloatValueChanged);
    
    _time.set("a random number (because... random)", 0);
    _time.setMin(1);
    _time.setMax(1000);
    gui.add(timeSlider.setup(_time));
    timeSlider.addListener(this, &VolumetricHybridApproach::shaderIntValueChanged);
}

#pragma mark update

void VolumetricHybridApproach::update(const string childName, const string fontName)
{
    bool nameChange = _childName == ofToUpper(childName);
    _childName = ofToUpper(childName);
    font.load(fontName, 148, true, true, true, 0.3, 300);
    renderName();
    
    if (shaderChanged || nameChange)
    {
//        _volumetricShader.setUniform1i("iterations", _iterations.get());
//        _volumetricShader.setUniform1i("iterationsMultiplier", _iterationMultiplier.get());
//        _volumetricShader.setUniform1i("volsteps", _volSteps.get());
//        _volumetricShader.setUniform1i("volstepsMultiplier", _volStepsMultiplier.get());
//        _volumetricShader.setUniform1f("formuparam", _forumparam.get());
//        _volumetricShader.setUniform1f("forumparamMultiplier", _forumparamMultiplier.get());
//        _volumetricShader.setUniform1f("stepsize", _stepsize.get());
//        _volumetricShader.setUniform1f("stepsizeMultiplier", _stepsizeMultiplier.get());
//        _volumetricShader.setUniform1f("zoom", _zoom.get());
//        _volumetricShader.setUniform1f("brightness", _brightness.get());
//        _volumetricShader.setUniform1f("darkmatter", _darkmatter.get());
//        _volumetricShader.setUniform1f("distfading", _distfading.get());
//        _volumetricShader.setUniform1f("distfadingMultiplier", _distfadingMultiplier.get());
//        _volumetricShader.setUniform1f("saturation", _saturation.get());
//        _volumetricShader.setUniform1f("tile", _tile.get());
//        _volumetricShader.setUniform1f("time", (float)_time.get());
        
//        _volumetricShader.setUniformTexture("tex0", _textRendered.getTexture(), 0);
        
        shaderChanged = false;
        ofLog() << " shader changed ";
    }

    
}

void VolumetricHybridApproach::renderName()
{
    const ofRectangle PRINT_SCALE_RECTANGLE = ofRectangle(392, 547, 2773, 952);
    float scale = _width / PRINT_DIMENSIONS_WIDTH;//used for scaling rect
    ofFbo txtRenderTmpLayer1;
    ofFbo txtRenderTmpLayer2;
    txtRenderTmpLayer1.allocate(_width, _height, GL_RGBA);
    txtRenderTmpLayer2.allocate(_width, _height, GL_RGBA);
    //    font.setLetterSpacing(ofMap((float)_childName.length(), 3.0, 12.0, 1.6, 1.0));
    ofRectangle boundsOrig = font.getStringBoundingBox(_childName, 0, 0);
    ofRectangle boundsNow = ofRectangle(boundsOrig);
    ofRectangle maxBounds = ofRectangle(PRINT_SCALE_RECTANGLE);
    
    maxBounds.scale(scale);
    
    boundsNow.scaleTo(maxBounds, OF_ASPECT_RATIO_KEEP, OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER, OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);
    
    float scaleForFontNow = boundsNow.width / boundsOrig.width;
    
    _typePathScale = scaleForFontNow;
    _textPositionRectangle = boundsNow;
    boundsOrig.scale(scaleForFontNow);
    
    txtRenderTmpLayer1.begin();
    ofClear(255, 0, 0, 255);
    ofPushMatrix();
    ofSetColor(255, 255, 255);
    ofTranslate(_textPositionRectangle.getLeft(), _textPositionRectangle.getBottom());
    ofScale(scaleForFontNow, scaleForFontNow);
    font.drawString(_childName, 0, 0);
    ofPopMatrix();

    txtRenderTmpLayer1.end();
//    _textRendered = txtRenderTmp;
    ofPixels tmpPixels;
    txtRenderTmpLayer1.readToPixels(tmpPixels);
    ofImage txtRenderForBlurImage = ofImage(tmpPixels);
    
    txtRenderTmpLayer1.begin();
    ofClear(255, 0, 0, 255);
        //temp
    _filter1->begin();
    txtRenderForBlurImage.draw(0, 0);
    _filter1->end();
    
    //smaller glow
    txtRenderTmpLayer1.end();
    
    txtRenderTmpLayer2.begin();
    
    _filter2->begin();
    txtRenderForBlurImage.draw(0, 0);
    _filter2->end();
    
    txtRenderTmpLayer2.end();
    
    _textRendered.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    txtRenderTmpLayer1.draw(0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    txtRenderTmpLayer2.draw(0, 0);
    _textRendered.end();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
}



#pragma mark draw

void VolumetricHybridApproach::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofTexture t = _textRendered.getTexture();
//    t.draw(0, 0);
//    t.bind();
    //_textRendered.draw(0, 0);
    _volumetricShader.setUniformTexture("tex0", t, 0);
    _volumetricShader.begin();
    t.draw(0, 0);
    //ofDrawRectangle(0, 0, _width, _height);
    
    _volumetricShader.end();
//    t.unbind();
}


