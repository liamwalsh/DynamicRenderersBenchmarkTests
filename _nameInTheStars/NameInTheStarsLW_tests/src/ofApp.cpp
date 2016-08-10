#include "ofApp.h"
#include "GiorgiaStars1.hpp"
#include "GiorgiaStars2.hpp"
#include "EdConcept1.hpp"
#include "NervaticaGlow_giorgia1.hpp"
#include "NervaticaGlow_giorgia2.hpp"
#include "NervaticaGlowSickness.hpp"
#include "VolumetricHybridApproach.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    _bgImage.load("images/bgPurple.png");
    _fgImage.load("images/nameStarsGiorgiaFg.png");
    
    //fonts
    
    fontsDir.open("fonts");
    fontsDir.allowExt("ttf");
    fontsDir.allowExt("otf");
    fontsDir.listDir();
    _fontIndex = 0;
    _renderFbo.allocate(_fgImage.getWidth(), _fgImage.getHeight());
    _xprtFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    namesCsv.load("TJH_Name_Door_Street.csv");
    _nameIndex = 1;//not zero, because that's the column names
    _currentName = namesCsv.getRow(_nameIndex).getString(0);
    _scaleFactor = 0.5;
    _rendererIndex = 0;
    //renderers
    
    string nervaticaSickness = "comingAlongNowIHope";
    renderers[nervaticaSickness] = new NervaticaGlowSickness();
    rendererTypes.push_back(nervaticaSickness);
    
//    string volumetricHybrid = "volumetricHybridApproach";
//    renderers[volumetricHybrid] = new VolumetricHybridApproach();
//    rendererTypes.push_back(volumetricHybrid);
    
//    string nervaticaKey2 = "NervaticaMotherFuckaGiorgiaComeOn";
//    renderers[nervaticaKey2] = new NervaticaGlow_giorgia2();
//    rendererTypes.push_back(nervaticaKey2);
//    
//    string nervaticaKey = "NervaticaMotherFucka";
//    renderers[nervaticaKey] = new NervaticaGlow_giorga1();
//    rendererTypes.push_back(nervaticaKey);
    
//    string keyE1 = "Ed1";
//    renderers[keyE1] = new EdConcept1();
//    rendererTypes.push_back(keyE1);
    
//    string keyG1 = "Giorgia1";
//    renderers[keyG1] = new GiorgiaStars1();
////    rendererTypes.push_back(keyG1);
//    
//    string keyG2 = "Giorgia2";
//    renderers[keyG2] = new GiorgiaStars2();
////    rendererTypes.push_back(keyG2);
    
    currentRenderer = renderers[ rendererTypes [_rendererIndex]];
    currentRenderer -> setup(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    
    _updateRequired = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(!_updateRequired)
    {
        return;
    }
    
    ofResetElapsedTimeCounter();
    
    //TODO : update renderer
    //TODO :: update with the name...  update with the font... etc
    
    currentRenderer -> update(_currentName, fontsDir.getFile(_fontIndex).getAbsolutePath());
    _updateRequired = false;
//    currentRenderer -> update();
}

void ofApp::switchRenderer(){
    currentRenderer -> dispose();
    currentRenderer = renderers[ rendererTypes [ _rendererIndex]];
    currentRenderer -> setup(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    _updateRequired = true;
}



void ofApp::nextLayout() {
    ofLog() << " next layout " ;
    if(_rendererIndex + 1 < rendererTypes.size())
    {
        _rendererIndex++;
        
    }
    else{
        _rendererIndex = 0;
        
    }
    switchRenderer();
}

void ofApp::prevLayout() {
    if(_rendererIndex >= 1)
    {
        _rendererIndex--;
    }
    else{
        _rendererIndex = rendererTypes.size() -1 ;
    }
    switchRenderer();
    
}

void ofApp::nextFont() {
    
    _fontIndex++;
    _fontIndex = _fontIndex % fontsDir.listDir();
    _updateRequired = true;
}
void ofApp::prevFont() {
    ofLog() << " prev font ";
    if(_fontIndex > 0)
    {
        _fontIndex--;
    }
    else{
        _fontIndex = fontsDir.listDir() - 1;
    }
}

void ofApp::prevName()
{
    
    if(_nameIndex > 2)
    {
        _nameIndex -= 1;
    }
    _currentName = namesCsv.getRow(_nameIndex).getString(0);
    _updateRequired = true;
}
void ofApp::nextName()
{
    if(_nameIndex < (namesCsv.getNumRows() - 1))
    {
        _nameIndex++;
    }
    else{
        _nameIndex = 1;
    }
    _currentName = namesCsv.getRow(_nameIndex).getString(0);
    _updateRequired = true;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    
    
    ofClear(0, 0, 0);
    
    //fbo begin
    
    _renderFbo.begin();
    ofClear(0, 0, 0, 0);
//    ofBackground(255, 0, 0);
        currentRenderer -> draw();
    
    _renderFbo.end();
    
    ofPushMatrix();
        ofScale(SCREEN_SCALE_FACTOR, SCREEN_SCALE_FACTOR);

        _bgImage.draw(0, 0);

    ofSetColor(255, 255, 255);
        _renderFbo.draw(0, 0);

        _fgImage.draw(0, 0);
    ofPopMatrix();
    
    auto timeTaken = ofGetElapsedTimeMillis();
    
    ofLog() << " Pedro's name in the stars; name " << _currentName << " drawTime " << timeTaken;
    
    if(_awaitingSave)
    {
        _xprtFbo.begin();
        ofClear(0, 0, 0);
        ofSetColor(255, 255, 255);
        
        _bgImage.draw(0, 0);
        _renderFbo.draw(0, 0);
        _fgImage.draw(0, 0);
        
        _xprtFbo.end();
        
        string child = "xprts/child_" + _currentName + ".png";
        ofPixels pix;
        _xprtFbo.readToPixels(pix);
        
        
        
        ofSaveImage(pix, child);
        
        _awaitingSave = (exportAll && _nameIndex <= maxTestExport);
        
    }
    
    if(currentRenderer->getDebugModeEnabled())
    {
        currentRenderer->drawDebug();
    }
    
    if(exportAll && _nameIndex <= maxTestExport)
    {
        nextName();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == OF_KEY_RIGHT)
    {
        //next layout
        nextLayout();
        return;
    }
    if(key == OF_KEY_LEFT)
    {
        prevLayout();
        return;
    }
    if(key == OF_KEY_UP)
    {
        prevFont();
        return;
    }
    if(key == OF_KEY_DOWN)
    {
        nextFont();
        return;
    }
    if(key == 'n')
    {
        prevName();
        return;
    }
    if(key == 'm')
    {
        nextName();
        return;
    }
    if(key == 'd')
    {
        currentRenderer->setDebugModeEnabled(!currentRenderer->getDebugModeEnabled());
        return;
    }
    if(key == 's')
    {
        //save it out
        _awaitingSave = true;
    }
    if(key == 'a')
    {
        exportAll = !exportAll;
        _awaitingSave = exportAll;
    }
    if(key == 'g')
    {
        
        string name = ofSystemTextBoxDialog("state yo' name", _currentName);
        _currentName = name;
        ofLog() << _currentName << ", " << name;
        _updateRequired = true;
        
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
