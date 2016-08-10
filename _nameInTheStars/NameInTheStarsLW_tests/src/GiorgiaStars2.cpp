//
//  GiorgiaStars2.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 24/05/2016.
//
//

#include "GiorgiaStars2.hpp"
#include "ofApp.h"

void GiorgiaStars2::draw()   {
    
    
    ofPushStyle();
    ofSetColor(0, 255, 0);
//    ofDrawCircle(200, 200, 200);
//    _drawFontFbo.draw(150, 1000);
    contourFinder.draw();
    ofPopStyle();
    
    
//    sdfImage.draw(0, 0);
    
    shader.begin();
    if (shaderChanged)
    {
        shader.setUniform1i("iterations", _iterations.get());
        shader.setUniform1i("iterationsMultiplier", _iterationMultiplier.get());
        shader.setUniform1i("volsteps", _volSteps.get());
        shader.setUniform1i("volstepsMultiplier", _volStepsMultiplier.get());
        shader.setUniform1f("formuparam", _forumparam.get());
        shader.setUniform1f("forumparamMultiplier", _forumparamMultiplier.get());
        shader.setUniform1f("stepsize", _stepsize.get());
        shader.setUniform1f("stepsizeMultiplier", _stepsizeMultiplier.get());
        shader.setUniform1f("zoom", _zoom.get());
        shader.setUniform1f("brightness", _brightness.get());
        shader.setUniform1f("darkmatter", _darkmatter.get());
        shader.setUniform1f("distfading", _distfading.get());
        shader.setUniform1f("distfadingMultiplier", _distfadingMultiplier.get());
        shader.setUniform1f("saturation", _saturation.get());
        shader.setUniform1f("tile", _tile.get());
        shader.setUniform1i("time", _time.get());
        shader.setUniform1f("paperWidth", sdfImage.getWidth());
        shader.setUniform1f("paperHeight", sdfImage.getHeight());
        shader.setUniformTexture("nameTexture", sdfImage.getTexture(), 1);
        
        shaderChanged = false;
        
    }
    
    ofDrawRectangle(0, 0, sdfImage.getWidth(), sdfImage.getHeight());
    shader.end();
    
//    font.drawString(_childName, 100, 500);
}

void GiorgiaStars2::drawDebug() {
    gui.draw();
}

void GiorgiaStars2::setup(float width, float height)
{
    quadWarper.setSize(width, height);
    
    sdfPixels.allocate((int)width, (int)height, 3);
    sdfImage.allocate((int)width, (int)height, OF_IMAGE_COLOR);
    shader.load("shaders/Giorgia2/stars.vert", "shaders/Giorgia2/stars.frag");
    shaderChanged = true;
    setupGui();
}

void GiorgiaStars2::setupGui() {
    gui.setup("solarPanel");
    gui.setSize(500, 500);
    gui.setDefaultWidth(500);
    
    _iterations.set("iterations (number of stars)", 0);
    _iterations.setMin(0);
    _iterations.setMax(30);
    gui.add(iterationSlider.setup(_iterations));
    iterationSlider.addListener(this, &GiorgiaStars2::shaderIntValueChanged);
    
    _iterationMultiplier.set("iteration Name Multiplier", 22);
    _iterationMultiplier.setMin(0);
    _iterationMultiplier.setMax(40);
    
    gui.add(iterationsMultiplierSlider.setup(_iterationMultiplier));
    iterationsMultiplierSlider.addListener(this, &GiorgiaStars2::shaderIntValueChanged);
    
    _volSteps.set("volume Steps (galaxy-ness) ", 22);
    _volSteps.setMax(100);
    _volSteps.setMin(0);
    
    gui.add(volSteps.setup(_volSteps));
    volSteps.addListener(this, &GiorgiaStars2::shaderIntValueChanged);
    
    _volStepsMultiplier.set("volume steps name multiplier", 0);
    _volStepsMultiplier.setMin(0);
    _volStepsMultiplier.setMax(100);
    gui.add(volStepsMultiplier.setup(_volStepsMultiplier));
    volStepsMultiplier.addListener(this, &GiorgiaStars2::shaderIntValueChanged);
    
    _forumparam.set("stary-ness (speckle)", 0.53);
    _forumparam.setMin(0.45);
    _forumparam.setMax(1.0);
    gui.add(forumparamSlider.setup(_forumparam));
    forumparamSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _forumparamMultiplier.set("speckle name multiplier", 0);
    _forumparamMultiplier.setMin(0);
    _forumparamMultiplier.setMax(0.1);
    gui.add(forumparamMultiplierSlider.setup(_forumparamMultiplier));
    _forumparamMultiplier.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _stepsize.set("step size (density)", 0.050);
    _stepsize.setMax(0.09);
    _stepsize.setMin(0.01);
    gui.add(stepsizeSlider.setup(_stepsize));
    stepsizeSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _stepsizeMultiplier.set("step size name multiplier", 0);
    _stepsizeMultiplier.setMax(0.2);
    _stepsizeMultiplier.setMin(0);
    gui.add(stepsizeMultiplierSlider.setup(_stepsizeMultiplier));
    stepsizeMultiplierSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _zoom.set("zoom (camera distance)", 0.8);
    _zoom.setMin(0.05);
    _zoom.setMax(100.0);
    gui.add(zoomSlider.setup(_zoom));
    zoomSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    //brightness, darkmatter, distfading, saturation, tile
    _brightness.set("brightness", 0.0015);
    _brightness.setMax(0.0085);
    _brightness.setMin(0.0005);
    gui.add(brightnessSlider.setup(_brightness));
    brightnessSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _darkmatter.set("darkmatter (steals light)", 0.3);
    _darkmatter.setMax(2000.0);
    _darkmatter.setMin(0.00);
    gui.add(darkmatterSlider.setup(_darkmatter));
    darkmatterSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _distfading.set("dist fading (more = bluer & brighter)", 0.76);
    _distfading.setMin(0.5); _distfading.setMax(0.95);
    gui.add(distFadingSlider.setup(_distfading));
    distFadingSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _distfadingMultiplier.set("dist fading name multiplier", 0);
    _distfadingMultiplier.setMin(0.0);
    _distfadingMultiplier.setMax(1.0);
    gui.add(distFadingMultiplierSlider.setup(_distfadingMultiplier));
    distFadingMultiplierSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _saturation.set("saturation", 0.8);
    _saturation.setMin(0.5);
    _saturation.setMax(0.99);
    gui.add(saturationSlider.setup(_saturation));
    saturationSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _tile.set("tile ratio (wrap)", 0.85);
    _tile.setMin(0.5);
    _tile.setMax(3.0);
    gui.add(tileSlider.setup(_tile));
    tileSlider.addListener(this, &GiorgiaStars2::shaderFloatValueChanged);
    
    _time.set("a random number (because... random)", 100);
    _time.setMin(1);
    _time.setMax(1000);
    gui.add(timeSlider.setup(_time));
    timeSlider.addListener(this, &GiorgiaStars2::shaderIntValueChanged);
    
    
    //haz settings
    if (ofFile::doesFileExist("settings.xml", true))
    {
        ofLog() << "foo";
        gui.loadFromFile("settings.xml");
    }
}

void GiorgiaStars2::shaderIntValueChanged(int &intVal)
{
    shaderChanged = true;
}

void GiorgiaStars2::shaderFloatValueChanged(float &floatVal)
{
    shaderChanged = true;
}


void GiorgiaStars2::update(const string childName, const string fontName)  {
    ofLog() << "giorgia stars  2 update, load font " << fontName;
    font.load(fontName, 400);
    _childName = childName;
    _drawFontFbo.clear();
    _drawFontFbo.allocate(quadWarper.getWidth(), quadWarper.getHeight(), GL_RGB);
    _drawFontFbo.begin();
    sdfPixels.setColor(ofColor(0));
    ofClear(0, 0, 0);
    ofPushMatrix();
    ofTranslate(400, 1500);//THIS IS JUST A GUESS REALLY
    ofRotateY(40);
    ofRotateZ(-10);
    ofSetColor(255, 255, 255);
    ofSetBackgroundColor(255, 0, 0);
    ofScale(1 / SCREEN_SCALE_FACTOR, 1 / SCREEN_SCALE_FACTOR);
    //TODO -rotate scale, warp etc
    font.drawString(_childName, 0, 100);//TODO - position this properly
    ofPopMatrix();
    _drawFontFbo.end();
    ofxCvGrayscaleImage grayScaleCvImg;
    ofPixels pixels;
    pixels.allocate(_drawFontFbo.getWidth(), _drawFontFbo.getHeight(), OF_PIXELS_RGB);
    _drawFontFbo.readToPixels(pixels);
    ofxCvColorImage colorCvImage;
    colorCvImage.setFromPixels(pixels);
    grayScaleCvImg = colorCvImage;
    
    grayScaleCvImg.threshold(80);
    int minBlobArea = 50 * 50;
    int maxBlobArea = grayScaleCvImg.width * grayScaleCvImg.height;
    int t1 = ofGetUnixTime();
    int result = contourFinder.findContours(grayScaleCvImg, minBlobArea, maxBlobArea, childName.size() * 4, true);
    
    auto iter = contourFinder.blobs.begin();
    int bCount = 0;
    float mDist = 70;
    while(iter != contourFinder.blobs.end())
    {
        ofxCvBlob blob = *iter;
        
        ofRectangle rect = blob.boundingRect;
                
        if(!blob.hole)
        {
            int l = (int)rect.getLeft();
            int r = (int)rect.getRight();
            int t = (int)rect.getTop();
            int b = (int)rect.getBottom();
            ofPoint p;
            ofPolyline polyLine;
            polyLine.addVertices(blob.pts);
            polyLine.close();
            for(int col = l; col<=r; col++)
            {
                for(int row = t; row<=b; row++)
                {
                    if(pixels.getColor(col, row).r > 0)
                    {
                        float minDist = mDist;
                        p.set(col, row);
                        auto closestPoint = polyLine.getClosestPoint(p);
                        minDist = min(p.distance(closestPoint), minDist);
                        
                        int r = (int)(255 * (mDist - minDist) / mDist);
                        ofColor color = sdfPixels.getColor(col, row) + ofColor(r, 0, 255);
                        
                        sdfPixels.setColor(col, row, color);
                    }
                }
            }
        }
        else{
            //TODO : do the same thing for the outside ring of the blob bounding rect
            rect.setFromCenter(blob.centroid.x, blob.centroid.y, rect.width * 2, rect.height * 2);
            ofPoint p;
            ofPolyline polyLine;
            polyLine.addVertices(blob.pts);
            polyLine.close();
            int l = (int)rect.getLeft();
            int r = (int)rect.getRight();
            int t = (int)rect.getTop();
            int b = (int)rect.getBottom();
            
            for(int col = l; col<=r; col++)
            {
                for(int row = t; row<=b; row++)
                {
                    if(pixels.getColor(col, row).r > 0)
                    {
                        float minDist = mDist;
                        p.set(col, row);
                        auto closestPoint = polyLine.getClosestPoint(p);
                        minDist = min(p.distance(closestPoint), minDist);
                        
                        int r = (int)(255 * (mDist - minDist) / mDist);
                        ofColor color = sdfPixels.getColor(col, row) + ofColor(r, 0, 255);
                        sdfPixels.setColor(col, row, color);
                    }
                }
            }
        }
            
        bCount++;
        
        iter++;
    }
    
    ofLog() << " blobs sorted out in " << (ofGetUnixTime() - t1);
    sdfImage.setFromPixels(sdfPixels);
    pixels.clear();
    
}


