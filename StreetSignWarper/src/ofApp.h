#ifndef TJH_Globals
#define TJH_Globals
const float PRINT_DIMENSIONS_WIDTH = 3596;
const float PRINT_DIMENSIONS_HEIGHT = 2657;
const float SCREEN_SCALE_FACTOR = 0.25;
#endif

#pragma once

#include "ofMain.h"
#include "ofxQuadWarper.h"
#include "ofxCsv.h"

class SignImage  {
public:
    ofImage topLeftImage;
    ofImage topCenterImage;
    ofImage topRightImage;
    
    ofImage centerLeftImage;
    ofImage centerCenterImage;
    ofImage centerRightImage;
    
    ofImage bottomLeftImage;
    ofImage bottomCenterImage;
    ofImage bottomRightImage;
    
    ofRectangle cornerSizeRect;
    ofRectangle centerSlice;
    
    ofRectangle textBounds;
    ofRectangle signBounds;
    ofRectangle renderSignCenterBounds;
    
    float textMargin;
    
    ofTrueTypeFont font;
    ofColor textColor;
    
    float numLines;
    
    string _streetName;
    
    void cropAndPopulate(ofImage sourceImage)
    {
//        ofLog() << " centerSlice " << centerSlice;
        float cornerWidth = cornerSizeRect.width;
        float cornerHeight = cornerSizeRect.height;
        ofRectangle templateSize = ofRectangle(0, 0, sourceImage.getWidth(), sourceImage.getHeight());
        centerSlice = ofRectangle(cornerSizeRect.width, cornerSizeRect.height, templateSize.width - (2 * cornerSizeRect.width), templateSize.height - (2 * cornerSizeRect.height));
        //top row
        topLeftImage.cropFrom(sourceImage, 0, 0, cornerWidth, cornerHeight);
        topCenterImage.cropFrom(sourceImage, cornerWidth, 0, centerSlice.width, cornerHeight );
        topRightImage.cropFrom(sourceImage, centerSlice.getRight(), 0, cornerWidth, cornerHeight );
        //middle row
        centerLeftImage.cropFrom(sourceImage, 0, cornerHeight, cornerWidth, centerSlice.height);
        centerCenterImage.cropFrom(sourceImage, cornerWidth, cornerHeight, centerSlice.width, centerSlice.height);
        centerRightImage.cropFrom(sourceImage, centerSlice.getRight(), centerSlice.getTop(), cornerWidth, centerSlice.height );
        
        bottomLeftImage.cropFrom(sourceImage, 0, centerSlice.getBottom(), cornerWidth, cornerHeight);
        bottomCenterImage.cropFrom(sourceImage, centerSlice.getLeft(), centerSlice.getBottom(), centerSlice.width, cornerHeight);
        bottomRightImage.cropFrom(sourceImage, centerSlice.getRight(), centerSlice.getBottom(), cornerWidth, cornerHeight);
        
    }
    
    void updateTypeBounds(string streetName) {
        const float maxTextLineWidth = 1200;
        ofPoint streetSignTopLeft = ofPoint(0, 0);
        
        ofRectangle typeBounds = font.getStringBoundingBox(streetName, 0, 0);
        
        
        float descender = font.getDescenderHeight();
        float ascender = font.getAscenderHeight();
        bool offset = abs(typeBounds.height) >= (ascender - 1);
//        ofLog() << streetName << " :: bounds = |" << typeBounds << "|, ascender " << ascender << ", descender " << descender << " line height total " << (ascender - descender) << " box " << font.getGlyphBBox() << " line height " << font.getLineHeight();
//        ofLog() << "offset ? " << offset;
        numLines = 0;
        if (typeBounds.getWidth() > maxTextLineWidth) {
            string newString = "";
            vector<string> streetSplit = ofSplitString(streetName, " ");
            vector<string>::iterator it = streetSplit.begin();
            
            newString = *it;
            ofRectangle nBounds = font.getStringBoundingBox(newString, 0, 0);
            
            for (it = streetSplit.begin() + 1; it < streetSplit.end(); it++)
            {
                
                string joiner = " ";
                string testString = newString + joiner + *it;
                ofRectangle testRect = font.getStringBoundingBox(testString, 0, 0);
                if (testRect.getWidth() > maxTextLineWidth)
                {
                    joiner = "\n";
                    numLines += 1.0;
                    offset = false;
                }
                newString += (joiner + *it);
                
            }
            streetName = newString;
            typeBounds = font.getStringBoundingBox(streetName, 0, 0, true);
            
        }
        
        textBounds = ofRectangle(streetSignTopLeft.x + textMargin + cornerSizeRect.width, streetSignTopLeft.y + textMargin + cornerSizeRect.height, typeBounds.width, typeBounds.height);
        if (offset)
        {
            //        textBounds.y += descender;
            typeBounds.height += descender;
            textBounds.height += descender;
        }
        signBounds = ofRectangle(streetSignTopLeft, typeBounds.width + (2 * (textMargin + cornerSizeRect.width)), typeBounds.height + (2 * (cornerSizeRect.height + textMargin)));
        
        renderSignCenterBounds = ofRectangle(streetSignTopLeft.x + cornerSizeRect.width, streetSignTopLeft.y + cornerSizeRect.height, typeBounds.width + (2 * textMargin), typeBounds.height + (2 * textMargin));
        _streetName = streetName;
    }
    
    void draw()
    {
        
        ofPoint streetSignTopLeft = ofPoint(0, 0);
        
        topLeftImage.draw(streetSignTopLeft);
        topCenterImage.resize(renderSignCenterBounds.width, cornerSizeRect.height);
        topCenterImage.draw(renderSignCenterBounds.x, streetSignTopLeft.y);
        topRightImage.draw(renderSignCenterBounds.getRight(), streetSignTopLeft.y);
        
        centerLeftImage.draw(streetSignTopLeft.x, renderSignCenterBounds.y, cornerSizeRect.width, renderSignCenterBounds.height);
        centerCenterImage.draw(renderSignCenterBounds);
        centerRightImage.draw(renderSignCenterBounds.getRight(), renderSignCenterBounds.getTop(), cornerSizeRect.width, renderSignCenterBounds.height);
        
        bottomLeftImage.draw(streetSignTopLeft.x, renderSignCenterBounds.getBottom());
        bottomCenterImage.draw(renderSignCenterBounds.getLeft(), renderSignCenterBounds.getBottom(), renderSignCenterBounds.width, cornerSizeRect.height);
        bottomRightImage.draw(renderSignCenterBounds.getRight(), renderSignCenterBounds.getBottom());
        
        ofPushStyle();
        ofSetColor(textColor);
        font.drawString(_streetName, textBounds.x, textBounds.getBottom() - (numLines * font.getLineHeight()));
        ofPopStyle();
    }
    
    
};

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
    
    private:
    //methods
        SignImage loadSignSettings(string locale);
        void      loadWarpSettings();
        void      saveWarpSettings();
        void      nextRow();
        void      prevRow();
    //variables
        ofxCsv dataCSV;
        SignImage activeSignImage;
        //TODO : rotation
        ofxQuadWarper quadWarper;
    
        ofFbo                   exportFbo;
        ofImage                 bgImage;
        ofImage                 fgImage;
        map<string, SignImage>  localisedSigns;
        string locale = "gb";
        string streetName;
        int rowIndex;
    //TODO : server
        int draggingWarpIndex;
        bool debugMode;
        bool updateRequired;
    
        ofVec2f _fboOffsetPos;
        float notificationAlpha;
    
};

