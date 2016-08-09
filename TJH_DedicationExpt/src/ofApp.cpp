#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0, 255);
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    faceImage.load("images/tomC.png");
    font.load("fonts/Serapicos.otf", 72, true, true, true, 0.3, 72);
    setupGui();
    shader.load("shaders/stars.vert", "shaders/stars.frag");
    shaderChanged = true;
    saveRequested = true;
}

void ofApp::setupGui() {
    gui.setup("solarPanel");
    gui.setSize(500, 500);
    gui.setDefaultWidth(500);
    
    _iterations.set("iterations (number of stars)", 17);
    _iterations.setMin(0);
    _iterations.setMax(30);
    gui.add(iterationSlider.setup(_iterations));
    iterationSlider.addListener(this, &ofApp::shaderIntValueChanged);
    
    _iterationMultiplier.set("iteration Face Multiplier", 0);
    _iterationMultiplier.setMin(0);
    _iterationMultiplier.setMax(15);
    
    gui.add(iterationsMultiplierSlider.setup(_iterationMultiplier));
    iterationsMultiplierSlider.addListener(this, &ofApp::shaderIntValueChanged);
    
    _volSteps.set("volume Steps (galaxy-ness) ", 30);
    _volSteps.setMax(100);
    _volSteps.setMin(0);
    
    gui.add(volSteps.setup(_volSteps));
    volSteps.addListener(this, &ofApp::shaderIntValueChanged);
    
    _volStepsMultiplier.set("volume steps Face multiplier", 0);
    _volStepsMultiplier.setMin(0);
    _volStepsMultiplier.setMax(100);
    gui.add(volStepsMultiplier.setup(_volStepsMultiplier));
    volStepsMultiplier.addListener(this, &ofApp::shaderIntValueChanged);
    
    _forumparam.set("stary-ness (speckle)", 0.53);
    _forumparam.setMin(0.45);
    _forumparam.setMax(1.0);
    gui.add(forumparamSlider.setup(_forumparam));
    forumparamSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _forumparamMultiplier.set("speckle name multiplier", 0);
    _forumparamMultiplier.setMin(0);
    _forumparamMultiplier.setMax(0.1);
    gui.add(forumparamMultiplierSlider.setup(_forumparamMultiplier));
    _forumparamMultiplier.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _stepsize.set("step size (density)", 0.17);
    _stepsize.setMax(0.9);
    _stepsize.setMin(0.00);
    gui.add(stepsizeSlider.setup(_stepsize));
    stepsizeSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _stepsizeMultiplier.set("step size name multiplier", 0);
    _stepsizeMultiplier.setMax(0.2);
    _stepsizeMultiplier.setMin(0);
    gui.add(stepsizeMultiplierSlider.setup(_stepsizeMultiplier));
    stepsizeMultiplierSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _zoom.set("zoom (camera distance)", 0.8);
    _zoom.setMin(0.05);
    _zoom.setMax(100.0);
    gui.add(zoomSlider.setup(_zoom));
    zoomSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    //brightness, darkmatter, distfading, saturation, tile
    _brightness.set("brightness", 0.0015);
    _brightness.setMax(0.0085);
    _brightness.setMin(0.0005);
    gui.add(brightnessSlider.setup(_brightness));
    brightnessSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _darkmatter.set("darkmatter (steals light)", 0.3);
    _darkmatter.setMax(0.99);
    _darkmatter.setMin(0.1);
    gui.add(darkmatterSlider.setup(_darkmatter));
    darkmatterSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _distfading.set("dist fading (more = bluer & brighter)", 0.73);
    _distfading.setMin(0.5); _distfading.setMax(0.95);
    gui.add(distFadingSlider.setup(_distfading));
    distFadingSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _distfadingMultiplier.set("dist fading name multiplier", 0);
    _distfadingMultiplier.setMin(0.0);
    _distfadingMultiplier.setMax(1.0);
    gui.add(distFadingMultiplierSlider.setup(_distfadingMultiplier));
    distFadingMultiplierSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    
    _saturation.set("saturation", 0.8);
    _saturation.setMin(0.5);
    _saturation.setMax(0.99);
    gui.add(saturationSlider.setup(_saturation));
    saturationSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _tile.set("tile ratio (wrap)", 0.85);
    _tile.setMin(0.5);
    _tile.setMax(3.0);
    gui.add(tileSlider.setup(_tile));
    tileSlider.addListener(this, &ofApp::shaderFloatValueChanged);
    
    _time.set("a random number (because... random)", 0);
    _time.setMin(1);
    _time.setMax(1000);
    gui.add(timeSlider.setup(_time));
    timeSlider.addListener(this, &ofApp::shaderIntValueChanged);
    
    //haz settings
    if (ofFile::doesFileExist("settings.xml", true))
    {
        ofLog() << "foo";
        gui.loadFromFile("settings.xml");
    }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    ofResetElapsedTimeCounter();
}

void ofApp::shaderIntValueChanged(int &intVal)
{
    shaderChanged = true;
}

void ofApp::shaderFloatValueChanged(float &floatVal)
{
    shaderChanged = true;
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofFbo saveReqFbo;
    
    if(saveRequested)
    {
        saveReqFbo.allocate(ofGetWidth(), ofGetHeight());
        saveReqFbo.begin();
    }
        
    faceImage.draw(0,0);
    shader.begin();
    ofTexture faceTex = faceImage.getTexture();
    
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
        shader.setUniform1f("time", (float)_time.get());
        
        shader.setUniformTexture("facesTexture", faceTex, 1);
        
        shaderChanged = false;
        
    }
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shader.end();
//    faceTex.draw(0, 0);
    
    ofSetColor(255, 255, 255, 177);
    font.drawString("", 100, 100);
    ofSetColor(255, 255, 255, 255);
    
    if (!hideGUI)
    {
        gui.draw();
    }
    if(saveRequested)
    {
        ofLog() <<" dedication spike, draw time " << ofGetElapsedTimeMillis();
        saveReqFbo.end();
        saveReqFbo.draw(0, 0);
        ofPixels savePix;
        saveReqFbo.readToPixels(savePix);
        ofSaveImage(savePix, "images/xprt.png");
        
        saveRequested = false;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key =='h')
    {
        hideGUI = !hideGUI;
    }
    if(key == '1')
    {
        faceImage.load("images/pedroSpace2.png");
        shaderChanged = true;
        _time.set(int(ofRandom(0, 10)));
    }
    if(key == '2')
    {
        faceImage.load("images/talOron.png");
        shaderChanged = true;
        _time.set(int(ofRandom(0, 10)));
    }
    if(key == '3')
    {
        faceImage.load("images/bean.png");
        shaderChanged = true;
        _time.set(int(ofRandom(0, 10)));
    }
    if(key == '4')
    {
        faceImage.load("images/tomC.png");
        shaderChanged = true;
        _time.set(int(ofRandom(0, 10)));
    }
    if(key == 's')
    {
        saveRequested = true;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
