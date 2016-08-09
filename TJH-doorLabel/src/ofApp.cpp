#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    basicLabelFont.load("fonts/pleyo.otf", 56, true, true, true, 0.3, 300);
    smallerLabelFont.load("fonts/pleyo.otf", 29, true, true, true, 0.3, 300);
    
    exportFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    maximumDoorArea = ofRectangle(2475, 449, 686, 843);
    numberLabelTarget = ofRectangle(2477, 454, 686, 297);
    
    dummyDataCsv.load("TJH_Name_Door_Street.csv");
    bgImage.load("images/bgImage.png");
    
    updateRequired = true;
    ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(!updateRequired)
    {
        return;
    }
    doorLabel = dummyDataCsv.getRow(csvRowIndex).getString(1);
    
    ofRectangle renderedLabelPosition;//this is empty now, it's set when we do things...
    string renderDoorString = doorLabel;//copy
    ofTrueTypeFont renderFont = basicLabelFont;
    
    
    bool firstLineIsNumber = !doorLabel.empty() && std::find_if(doorLabel.begin(),
                                                                         doorLabel.end(), [](char c) { return !std::isdigit(c); }) == doorLabel.end();
    
    if(firstLineIsNumber || doorLabel.length() <= 5)
    {
        ofRectangle thisRect = basicLabelFont.getStringBoundingBox(doorLabel, 0, 0);
        thisRect.alignTo(numberLabelTarget, ofAlignHorz::OF_ALIGN_HORZ_CENTER, ofAlignVert::OF_ALIGN_VERT_CENTER, ofAlignHorz::OF_ALIGN_HORZ_CENTER, ofAlignVert::OF_ALIGN_VERT_CENTER);
        renderedLabelPosition = thisRect;
        
    }
    else{
        //it's a string
        //does it fit
        renderFont = smallerLabelFont;
        ofRectangle typeBounds = renderFont.getStringBoundingBox(renderDoorString, 0, 0, true);
        float descender = renderFont.getDescenderHeight();
        float ascender = renderFont.getAscenderHeight();
        bool offset = abs(typeBounds.height) >= (ascender - 1);
        int numLines = 0;
        if(typeBounds.width > maximumDoorArea.width)
        {
            string newString = "";
            vector<string> doorSplit = ofSplitString(doorLabel, " ");
            vector<string>::iterator it = doorSplit.begin();
            
            newString = *it;
            ofRectangle nBounds = renderFont.getStringBoundingBox(newString, 0, 0);
            for (it = doorSplit.begin() + 1; it < doorSplit.end(); it++)
            {
                string joiner = " ";
                string testString = newString + joiner + *it;
                ofRectangle testRect = renderFont.getStringBoundingBox(testString, 0, 0);
                if(testRect.width > maximumDoorArea.width)
                {
                    joiner = "\n";
                    numLines += 1;
                    offset = false;
                    
                }
                newString += (joiner + *it);
            }
            renderDoorString = newString;
            typeBounds = renderFont.getStringBoundingBox(renderDoorString, 0, 0, true);
            
        }
    }//end else (not a number)
    
    exportFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofSetColor(255, 0, 0);
    
    ofDrawRectangle(maximumDoorArea);
    ofSetColor(0, 255, 0);
    ofDrawRectangle(renderedLabelPosition);
    ofSetColor(0, 0, 0);
    renderFont.drawString(renderDoorString, renderedLabelPosition.getLeft(), renderedLabelPosition.getBottom());
    ofPopStyle();
    exportFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPushMatrix();
    
    ofScale(SCREEN_SCALE_FACTOR, SCREEN_SCALE_FACTOR);
    bgImage.draw(0, 0);
    exportFbo.draw(0, 0);
    ofPopMatrix();
    ofLog() << " door name draw " << ofGetElapsedTimeMillis();
    ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_DOWN)
    {
        csvRowIndex += csvRowIndex < dummyDataCsv.getNumRows() - 1 ? 1 : 0;
        updateRequired = true;
    }
    if(key == OF_KEY_UP)
    {
        csvRowIndex -= csvRowIndex > 1 ? 1 : 0;
        updateRequired = true;
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
