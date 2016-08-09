#include "ofApp.h"

#pragma mark setup
//--------------------------------------------------------------
void ofApp::setup(){
    
    posterFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    exportFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT);
    csvRowIndex = 1;
    dummyDataCsv.load("TJH_Name_Door_Street.csv");
    //data stuff
    countryLookUp["US"] = "United States";
    countryLookUp["GB"] = "United Kingdom";
    countryLookUp["UK"] = "United Kingdom";
    countryLookUp["IT"] = "Italy";
    countryLookUp["DE"] = "Deutschland";
    //TODO: externalise
    setupGeometry();
    setupLoadImages();
    setupTypefaces();
    updateRequired = true;
    ofResetElapsedTimeCounter();
}

void ofApp::setupTypefaces()
{
    rainbowFonts.reserve(7);
    ofTrueTypeFont pleyo11;
    pleyo11.load("fonts/Pleyo.otf", 11, true, true, true, 0.3, 300);
    pleyo11.setLetterSpacing(1.5);
    ofTrueTypeFont pleyo16;
    pleyo16.load("fonts/Pleyo.otf", 16, true, true, true, 0.3, 300);
    pleyo16.setLetterSpacing(1.5);
    ofTrueTypeFont nervaticaBold16;
    nervaticaBold16.load("fonts/NervaticaBold.otf", 16.6, true, true, true, 0.3, 300);
    nervaticaBold16.setLetterSpacing(1.5);
    ofTrueTypeFont nervaticaBold18;
    nervaticaBold18.load("fonts/NervaticaBold.otf", 18.5, true, true, true, 0.3, 300);
    nervaticaBold18.setLetterSpacing(1.5);
    
    rainbowFonts = {pleyo11, nervaticaBold16, pleyo16, nervaticaBold18, pleyo11, nervaticaBold16, pleyo11, nervaticaBold18};
    
    textColors = {
        ofColor::fromHex(0xffb78f),
        ofColor::fromHex(0xffb75b),
        ofColor::fromHex(0xffb8c8),
        ofColor::fromHex(0xd89eff),
        ofColor::fromHex(0xcc81ff),
        ofColor::fromHex(0xac9bfe),
        ofColor::fromHex(0xb5b3ff),
        ofColor::fromHex(0x8aaaff),
        ofColor::fromHex(0xadd8ff)
                    };
    
    ofTrueTypeFont nameFont1;
    nameFont1.load("fonts/Pleyo.otf", 50, true, true, true, 0.3, 300);
    
    ofTrueTypeFont nameFont2;
    nameFont2.load("fonts/Pleyo.otf", 45, true, true, true, 0.3, 300);
    
    ofTrueTypeFont nameFont3;
    nameFont3.load("fonts/Pleyo.otf", 40, true, true, true, 0.3, 300);
    
    ofTrueTypeFont nameFont4;
    nameFont4.load("fonts/Pleyo.otf", 35, true, true, true, 0.3, 300);
    
    ofTrueTypeFont nameFont5;
    nameFont5.load("fonts/Pleyo.otf", 30, true, true, true, 0.3, 300);
    
    ofTrueTypeFont nameFont6;
    nameFont6.load("fonts/Pleyo.otf", 30, true, true, true, 0.3, 300);
    
    ofTrueTypeFont nameFont7;
    nameFont7.load("fonts/Pleyo.otf", 30, true, true, true, 0.3, 300);
    
    nameFonts = {nameFont1, nameFont1, nameFont1, nameFont2, nameFont3, nameFont4, nameFont5, nameFont6, nameFont7};
    
}

void ofApp::setupGeometry()
{
    
    circleCenterPoint = ofPoint(1788, 2002);
    //these are distance from origin to text baseline
    rainbowRingRadii.reserve(9);
    rainbowRingRadii.push_back(280);
    rainbowRingRadii.push_back(385);
    rainbowRingRadii.push_back(503);
    rainbowRingRadii.push_back(616);
    rainbowRingRadii.push_back(721);
    rainbowRingRadii.push_back(797);
    rainbowRingRadii.push_back(895);//ok
    rainbowRingRadii.push_back(980);
    
    nameRingRadii = {0, 0, 781, 857,963, 963, 1022, 1143};
    
    ofLog() << " rainbow radii " << rainbowRingRadii.size() << ", vs " << nameRingRadii.size();
    
}

void ofApp::setupLoadImages()
{
    
    backgroundImage = ofImage("images/posterBG_print.png");
    
    foregroundImages["type-ii_boy_print"] = ofImage("images/foregrounds/posterFG_type-ii_boy_print.png");
    foregroundImages["type-iii_boy_print"] = ofImage("images/foregrounds/posterFG_type-iii_boy_print.png");
    foregroundImages["type-iv_boy_print"] = ofImage("images/foregrounds/posterFG_type-iv_boy_print.png");
    
    foregroundImages["type-ii_girl_print"] = ofImage("images/foregrounds/posterFG_type-ii_girl_print.png");
    foregroundImages["type-iii_girl_print"] = ofImage("images/foregrounds/posterFG_type-iii_girl_print.png");
    foregroundImages["type-iv_girl_print"] = ofImage("images/foregrounds/posterFG_type-iv_girl_print.png");
    
    for(int i = 0; i<=9; i++)
    {
        ofImage ringImage = ofImage("images/rings/" + ofToString((i)) + "Layers.png");
        rainbows.push_back(ringImage);
    }
    
}

#pragma mark update
//--------------------------------------------------------------
void ofApp::update(){
    if(!updateRequired)
    {
        return;//early exit
    }
    else{
        addressComponents.clear();
        addressComponents.reserve(9);
        addressComponents = {"The universe",
            "solar system",
            "earth",
            countryLookUp[dummyDataCsv.getRow(csvRowIndex).getString(5)]
        };
        
        //ok, how to get component 4
        if(dummyDataCsv.getRow(csvRowIndex).getString(4).length() > 0)
        {
            addressComponents.push_back(dummyDataCsv.getRow(csvRowIndex).getString(4));
        }
        if(dummyDataCsv.getRow(csvRowIndex).getString(3).length() > 0)
        {
            addressComponents.push_back(dummyDataCsv.getRow(csvRowIndex).getString(3));
        }
        //if first line of address is a number, combine with 2nd line
        string firstLineOfAddress = dummyDataCsv.getRow(csvRowIndex).getString(1);
        bool firstLineIsNumber = !firstLineOfAddress.empty() && std::find_if(firstLineOfAddress.begin(),
                                                            firstLineOfAddress.end(), [](char c) { return !std::isdigit(c); }) == firstLineOfAddress.end();
        
        if(firstLineIsNumber || firstLineOfAddress.length() <= 5)
        {
            
            string compositeFirstLine = firstLineOfAddress + " " + dummyDataCsv.getRow(csvRowIndex).getString(2);
            addressComponents.push_back(compositeFirstLine);
        }
        else{
            addressComponents.push_back(dummyDataCsv.getRow(csvRowIndex).getString(2));
            addressComponents.push_back(firstLineOfAddress);
        }
        
        //figure out foreground image
        string gender = dummyDataCsv.getRow(csvRowIndex).getString(6);
        string phototype = dummyDataCsv.getRow(csvRowIndex).getString(7);
        string lookupKey = phototype + "_" + gender + "_print";
        foregroundImage = foregroundImages[lookupKey];
        drawPoster();
        
    }
}

float ofApp::getArcAngleForLength(float stringLength, float radius)
{
    //we need to halve this to figure out when and where
    float angle = stringLength / radius; //because maths
    //also this is all radians
    return angle;
}

#pragma mark draw

void ofApp::drawPoster()
{
    posterFbo.begin();
    ofClear(0, 0, 0, 0);
    
    //TODO : figure out which rainbow to draw
    int ringIndex = addressComponents.size() - 1;
    
    rainbows[ringIndex + 2].draw(0, 0);
    for(int i = 0; i<= ringIndex; i++)
    {
        drawRing(i);
    }
    drawName();
    posterFbo.end();
    updateRequired = false;
}

ofVec2f ofApp::getTangent(float pos, ofPolyline curve)
{
    static float sub = 0.01;
    float min = pos - sub >= 0 ? pos - sub: 0;
    float max = pos + sub <= 1 ? pos + sub : 1;
    ofPoint t1 = curve.getPointAtLength(min);
    ofPoint t2 = curve.getPointAtLength(max);
    
    ofLog() << " tangent " << t2 - t1 << ", vs " << curve.getTangentAtIndexInterpolated(pos);
    return t2 - t1;
}

void ofApp::drawRing(int index)
{
    ofTrueTypeFont font = rainbowFonts[index];
    float radius = rainbowRingRadii[index];
    
    string addressBit = addressComponents[index];
    auto bitRect = font.getStringBoundingBox(addressBit, 0, 0);
    float arcAngle = getArcAngleForLength(bitRect.width, rainbowRingRadii[index]);
    float beginAngle = ofRadToDeg(-arcAngle / 2);
    float endAngle = ofRadToDeg(arcAngle / 2);
    ofPolyline p;
    //the 1.05 is because it's a bit wonky
    
    p.arc(circleCenterPoint, radius * 1.05, radius, 270 - ofRadToDeg(arcAngle / 2), 270 + ofRadToDeg(arcAngle / 2), true, 100);
    
    int runningCount = 0;
    
    ofRectangle runningStringDimensions;
    
    ofPushStyle();
    ofSetColor(textColors[index]);
    
    for(char &c : addressBit)
    {
        int nVal = runningCount + 1;
        string runningString = addressBit.substr(0, nVal);
        string thisCharString = ofToString(c);
        ofRectangle  thisCharDimensions = font.getStringBoundingBox(thisCharString, 0, 0);
        runningStringDimensions = font.getStringBoundingBox(runningString, 0, 0);
        float diff = runningStringDimensions.width - thisCharDimensions.width;
        float dPos = diff / bitRect.width;
        ofVec2f positionOnCurve = p.getPointAtPercent(dPos);
        float rot = ofMap(dPos, 0, 1.0, beginAngle, endAngle);
        
        float debugWidth = bitRect.width;
        
        ofPushMatrix();
        ofTranslate(positionOnCurve.x, positionOnCurve.y);
        ofRotate(rot, 0, 0, 1);
        font.drawString(thisCharString, 0, 0);
        
        ofPopMatrix();
        runningCount++;
    }
    
    ofPopStyle();
    
}

void ofApp::drawName()
{
    int ringIndex = addressComponents.size() - 1;
    float nameArcRadius = nameRingRadii[ringIndex];
    
    ofTrueTypeFont nameFont = nameFonts[ringIndex];
    
    string nameString = dummyDataCsv.getRow(csvRowIndex).getString(0) + "'S House";
    auto nameRect = nameFont.getStringBoundingBox(nameString, 0, 0);
    float arcAngleRadians = getArcAngleForLength(nameRect.width, nameArcRadius);
    float beginAngle = ofRadToDeg(-arcAngleRadians / 2);
    float endAngle = ofRadToDeg(arcAngleRadians / 2);
    
    ofPolyline p;
    //again, scale up due to squished circle Pedro drawing
    p.arc(circleCenterPoint, nameArcRadius * 1.05, nameArcRadius, 270 + beginAngle, 270 + endAngle, true, 100);
    
    int runningCount = 0;
    
    ofRectangle runningStringDimensions;
    
    ofPushStyle();
    ofSetColor(ofColor::fromHex(0xf3eee9));
    
    for(char &c : nameString)
    {
        int nVal = runningCount + 1;
        string runningString = nameString.substr(0, nVal);
        string thisCharString = ofToString(c);
        ofRectangle  thisCharDimensions = nameFont.getStringBoundingBox(thisCharString, 0, 0);
        runningStringDimensions = nameFont.getStringBoundingBox(runningString, 0, 0);
        float diff = runningStringDimensions.width - thisCharDimensions.width;
        float dPos = diff / nameRect.width;
        ofVec2f positionOnCurve = p.getPointAtPercent(dPos);
        float rot = ofMap(dPos, 0, 1.0, beginAngle, endAngle);
        
        ofPushMatrix();
        ofTranslate(positionOnCurve.x, positionOnCurve.y);
        ofRotate(rot, 0, 0, 1);
        nameFont.drawString(thisCharString, 0, 0);
        
        ofPopMatrix();
        runningCount++;
        
    }
    
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPushMatrix();
    ofScale(SCREEN_SCALE_FACTOR, SCREEN_SCALE_FACTOR);
    backgroundImage.draw(0, 0);
    posterFbo.draw(0, 0);
    foregroundImage.draw(0, 0);
    ofPopMatrix();
    ofLog() << " end of book bedroom poster draw time " << ofGetElapsedTimeMillis();
    csvNextRow();
    ofResetElapsedTimeCounter();
}
#pragma mark event handling

void ofApp::csvNextRow()
{
    if(csvRowIndex < (dummyDataCsv.getNumRows() - 1))
    {
        csvRowIndex++;
    }
    updateRequired = true;
}

void ofApp::csvPrevRow()
{
    if(csvRowIndex > 2)
    {
        csvRowIndex--;
    }
    updateRequired = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == OF_KEY_DOWN)
    {
        csvNextRow();
    }
    else if (key == OF_KEY_UP)
    {
        csvPrevRow();
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
