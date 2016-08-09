#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    _renderFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
//    renderer = *new TownDashboardRenderer();
    
    townNameChanged = true;
    dataCSV.load("TJH_Name_Door_Street.csv");
    _rowIndex = 1; // this is not zero as that is the titles
    townName = dataCSV.getRow(_rowIndex).getString(3);
    
//    _bgImage = ofImage("images/bgNew.png");
    _fgImage = ofImage("images/fgNew.png");
    saveReq = false;
    _saveQueueEnabled = true;
    ofSetFrameRate(120);
    ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void ofApp::update(){
    if(townNameChanged)
    {
//
        renderer.onTownNameChanged(townName, locale);
        townNameChanged = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0, 0, 0);
    
    //render custom things into an FBO here
    _renderFbo.begin();
    ofClear(0, 0, 0);
    
//        _bgImage.draw(0, 0);
        //rendered FBO
        renderer._exportFbo.draw(0, 0);
        //glass etc that goes on top
        _fgImage.draw(0,0);
    
    _renderFbo.end();
    ofPushMatrix();
    ofScale(SCREEN_SCALE_FACTOR, SCREEN_SCALE_FACTOR);
    _renderFbo.draw(0, 0);
    ofPopMatrix();
    
    if(saveReq || _saveQueueEnabled)
    {
//        ofPixels pix;
//        _renderFbo.readToPixels(pix);
//        ofSaveImage(pix, "xprts/" + townName + ".png");
        saveReq = false;
    }
    
    if(_saveQueueEnabled)
    {
        //ofDrawBitmapString("saved image " + ofToString(_rowIndex) + " " + townName , 50, 50);
        ofLog() << " town dashboard, drew " << townName << " draw time " << (ofGetElapsedTimeMillis());
        nextRow();
        ofResetElapsedTimeCounter();
    }
}

void ofApp::nextRow()
{
    _rowIndex = min(_rowIndex + 1, (int)dataCSV.getNumRows());
    townName = dataCSV.getRow(_rowIndex).getString(3);
    townNameChanged = true;
}
void ofApp::prevRow()
{
    _rowIndex = max(_rowIndex -1, 0);
    townName = dataCSV.getRow(_rowIndex).getString(3);
    townNameChanged = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_DOWN)
    {
        nextRow();
    }
    if (key == OF_KEY_UP)
    {
        prevRow();
    }
    if(key == 's')
    {
        saveReq = true;
    }
    if(key == '1')
    {
        locale = "en";
        townNameChanged = true;
    }
    if(key == '2')
    {
        locale = "es";
        townNameChanged = true;
    }
    if(key == '3')
    {
        locale = "fr";
        townNameChanged = true;
    }
    if(key == '4')
    {
        locale = "de";
        townNameChanged = true;
    }
    if(key == '5')
    {
        locale = "it";
        townNameChanged = true;
    }
    if(key == 'a')
    {
        _saveQueueEnabled = !_saveQueueEnabled;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
