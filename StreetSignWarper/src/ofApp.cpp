#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    _fboOffsetPos = ofVec2f(590, 723);//TODO: this is temporary stand-in
    
    dataCSV.load("TJH_Name_Door_Street.csv");
    rowIndex = 1; // this is not zero as that is the titles
    streetName = dataCSV.getRow(rowIndex).getString(2);
    ofDirectory locales = ofDirectory("locales");
    exportFbo.allocate(PRINT_DIMENSIONS_WIDTH, PRINT_DIMENSIONS_HEIGHT, GL_RGBA);
    locales.listDir();
    for(int i = 0; i<locales.size(); i++)
    {
        auto path = locales.getPath(i);
        ofFile f = ofFile(path);
        if(f.isDirectory())
        {
            string localPath = path.substr(path.find_last_of("/") + 1, path.length());
            SignImage signImageData = loadSignSettings(localPath);
            localisedSigns[localPath] = signImageData;
            
        }
    }
    activeSignImage = localisedSigns[locale];
    loadWarpSettings();
    
    bgImage.load("bgTemplate.png");
    fgImage.load("fgTemplate.png");
    draggingWarpIndex = -1;
    
    updateRequired = true;
    ofResetElapsedTimeCounter();
}

SignImage ofApp::loadSignSettings(string locale)
{
    SignImage signImage;
    ofFile settingsFile;
    settingsFile.open("locales/" + locale + "/signSettings.xml");
    ofBuffer buffer = settingsFile.readToBuffer();
    ofXml signSettings;
    signSettings.loadFromBuffer(buffer);
    signSettings.setTo("signSettings");
    int w = signSettings.getIntValue("cornerSize/width");
    int h = signSettings.getIntValue("cornerSize/height");
    float typeSize = signSettings.getFloatValue("fontSize");
    ofImage signTemplate = ofImage("locales/" + locale + "/signTemplate.png");
    
    signImage.font.load("locales/" + locale + "/font.otf", typeSize, true, true, true, 0.3f, 300);
    auto rect = signImage.font.getStringBoundingBox(streetName, 0, 0);
    
    
    signImage.textMargin = signSettings.getFloatValue("textMargin");
    string hex = signSettings.getValue("textColor");
    signImage.textColor.setHex(ofHexToInt(hex));
    //image
    signImage.cornerSizeRect = ofRectangle(0, 0, w, h);
    signImage.cropAndPopulate(signTemplate);//slice and dice
    
    return signImage;
}

void ofApp::loadWarpSettings()
{
    ofFile warpSettingsFile;
    warpSettingsFile.open("warpSettings.xml");
    if(!warpSettingsFile.exists())
    {
        ofLog() << " no warp settings file";
    }
    else{
        ofBuffer buffer = warpSettingsFile.readToBuffer();
        ofXml warpSettingsXML;
        warpSettingsXML.loadFromBuffer(buffer);
        warpSettingsXML.setTo("warpSettings");
        for(int i = 0; i<4; i++)
        {
            string childName = "t" + ofToString(i);
            warpSettingsXML.setTo(childName);
            quadWarper.setPoint(i, ofToFloat( warpSettingsXML.getAttribute("x")), ofToFloat(warpSettingsXML.getAttribute("y")));
            warpSettingsXML.setToParent();
        }
    }
}


//--------------------------------------------------------------
#pragma mark UPDATE
void ofApp::saveWarpSettings()
{
    ofXml warpSettingsXML;
    warpSettingsXML.addChild("warpSettings");
    warpSettingsXML.setTo("warpSettings");
    for (int i = 0; i<4; i++)
    {
        //here childName is not an actual child, just the xml node
        string childName = "t" + ofToString(i);
        ofVec2f q = quadWarper.getPoint(i);
        warpSettingsXML.addChild(childName);
        warpSettingsXML.setTo(childName);
        
        warpSettingsXML.setAttribute("x", ofToString(q.x));
        warpSettingsXML.setAttribute("y", ofToString(q.y));
        warpSettingsXML.setToParent();
    }
    
    warpSettingsXML.save("warpSettings.xml");
    
}

void ofApp::update(){
    
    if(!updateRequired)
    {
        notificationAlpha *= 0.99;
        notificationAlpha = notificationAlpha < 0.001 ? 0 : notificationAlpha;
        //early exit
        return;
    }
    
    activeSignImage = localisedSigns[locale];
    
    activeSignImage.updateTypeBounds(streetName);
    updateRequired = false;
}

void ofApp::nextRow()
{
    rowIndex = min(rowIndex + 1, (int)dataCSV.getNumRows());
    streetName = dataCSV.getRow(rowIndex).getString(2);
    updateRequired = true;
}
void ofApp::prevRow()
{
    rowIndex = max(rowIndex -1, 0);
    streetName = dataCSV.getRow(rowIndex).getString(2);
    updateRequired = true;
}

#pragma mark DRAW
//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0, 0, 0);
    //render sign to an fbo
    exportFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushMatrix();
    ofTranslate(_fboOffsetPos.x, _fboOffsetPos.y);
    ofRotate(-25, 0, 0, 1);
    quadWarper.push();
    activeSignImage.draw();
    quadWarper.pop();
    ofPopMatrix();
    exportFbo.end();
    //
    ofPushMatrix();
    ofScale(SCREEN_SCALE_FACTOR, SCREEN_SCALE_FACTOR);
    
    bgImage.draw(0, 0);
    
    exportFbo.draw(0, 0);
    fgImage.draw(0, 0);
    
    if(notificationAlpha > 0.001)
    {
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        
        ofSetColor(255, 255, 255, notificationAlpha * 255);
        ofDrawBitmapString("locale " + locale, 50, 50);
        ofPopStyle();
    }
    ofPopMatrix();
    
    ofLog() << " street sign " << streetName << " draw time " << ofGetElapsedTimeMillis();
    nextRow();
    ofResetElapsedTimeCounter();
    
    
}
#pragma mark EVENT HANDLING
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_DOWN)
    {
        nextRow();
        return;
    }
    if (key == OF_KEY_UP)
    {
        prevRow();
        return;
    }
    if(key == 'd')
    {
        debugMode = !debugMode;
        return;
    }
    if (key == 'r')
    {
        
        quadWarper.setPoint(0, 0, 0);
        quadWarper.setPoint(1, 1, 0);
        quadWarper.setPoint(2, 1, 1);
        quadWarper.setPoint(3, 0, 1);
    }
    if(key == '8')
    {
        locale = "us";
        updateRequired = true;
        notificationAlpha = 1.0;
    }
    if(key == '7')
    {
        locale = "it";
        updateRequired = true;
        notificationAlpha = 1.0;
    }
    if(key == '6')
    {
        locale = "gb";
        updateRequired = true;
        notificationAlpha = 1.0;
    }
    if(key == '5')
    {
        locale = "fr";
        updateRequired = true;
        notificationAlpha = 1.0;
    }
    if(key == '4')
    {
        locale = "es";
        updateRequired = true;
        notificationAlpha = 1.0;
    }
    if(key == '3')
    {
        locale = "de";
        updateRequired = true;
        notificationAlpha = 1.0;
    }
    if(key == '2')
    {
        locale = "ca";
        updateRequired = true;
        notificationAlpha = 1.0;
    }
    if(key == '1')
    {
        locale = "au";
        updateRequired = true;
        notificationAlpha = 1.0;
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
    if(!debugMode)
    {
        return;
    }
    if(draggingWarpIndex > -1) {
        
        ofVec2f mP = ofVec2f(x, y) - _fboOffsetPos ;
        mP *= 1 / SCREEN_SCALE_FACTOR;
        
//        quadWarper.setPoint(draggingWarpIndex, mP.x / activeSignImage.signBounds.width, mP.y / activeSignImage.signBounds.height);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    if(!debugMode)
    {
        return;
    }
    
    ofVec2f m = ofVec2f(x, y);
    ofLog() << "m before scale ( " << SCREEN_SCALE_FACTOR << " ) " << m;
    m *= ( 1 /  SCREEN_SCALE_FACTOR );
    ofLog() << " ma after scale " << m;
    ofVec2f signVec = ofVec2f(activeSignImage.signBounds.width, activeSignImage.signBounds.height);
    float maxDist = 10.0;
    for(int i = 0; i<4; i++)
    {
        ofVec2f n = quadWarper.getPoint(i);
        ofVec2f p = _fboOffsetPos + ofVec2f(n.x * activeSignImage.signBounds.width, n.y * activeSignImage.signBounds.height);
        
        ofLog() << " mouse click @ " << m << " dist from " << p << " is " << (p-m).length();
        if( abs( (p - m).length()) < maxDist)
        {
            draggingWarpIndex = i;
            ofLog() <<" found warp index " << i;
            return;
        }
    }
    ofLog() << "warp index not found " << draggingWarpIndex;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    draggingWarpIndex = -1;
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
