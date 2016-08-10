//
//  BrushWithGlowAndCirclePack.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 22/06/2016.
//
//

#include "BrushWithGlowAndCirclePack.hpp"

BrushWithGlowAndCirclePack::BrushWithGlowAndCirclePack(ofTexture pointTexture, float density, float fboWidth, float fboHeight, float insideShapeConstraint){
    //defaults constructor
    _insideShapeConstraint = insideShapeConstraint;
    brushOutlineScatterX = 100.0;
    brushOutlineScatterY = 100.0;
    
    pointTexture4Shader = pointTexture;
    pathVertexSkip = 10;
    _fboWidth = fboWidth;
    _fboHeight = fboHeight;
    textureAlpha = 1.0;
    textureWhitespaceScale = 2.0;
    minCircleSize = 1.0;
    maxCircleSize = 8.0;
    circleGrowthRate = maxCircleSize / 4.0;//TODO
    circleDensity = density;//not sure what to do with this yet
    baseColor = ofColor::white;
    alphaChannelFbo.allocate(fboWidth, fboHeight);
    pointPositionsVbo = ofVbo();
    hasGlow = false;
    blendMode = OF_BLENDMODE_ALPHA;//this isn't for the glow, just the basic thing
    _cirlceDensityMultiplier = 1.0;
}

void BrushWithGlowAndCirclePack::setGlowPass(GaussianBlurFilter *filter)
{
    hasGlow = true;
    blurFilter = filter;
    if(!blackBlurFbo.isAllocated())
    {
        
        blackBlurFbo.allocate(_fboWidth, _fboHeight);
    }
}

void BrushWithGlowAndCirclePack::populateFromPixels(const ofPixels &pix, ofRectangle activeBounds)
{

    int numCirclesToAdd = (int)activeBounds.getArea() / (maxCircleSize * maxCircleSize * M_PI);

    float densityResult = 0.0;
    float circleDensityTarget = circleDensity * _cirlceDensityMultiplier;
    while (densityResult <= circleDensityTarget)
    {
        int createdCircles = 0;
        for(int circleStep = 0; circleStep<numCirclesToAdd; circleStep++)
        {
            for(int i = 0; i<circles.size(); i++)
            {
                circles[i]->update();
                
                circles[i]->overlap(circles);
            }
            createdCircles = addCircles(numCirclesToAdd / 10, pix, activeBounds);
            densityResult = (float)circles.size() / numCirclesToAdd;
            
            if(densityResult > circleDensityTarget)
            {
                break;
            }
        }
    }
    sizes.clear();
    points.clear();
    floatColors.clear();
    
    ofFloatColor col = ofFloatColor((float)baseColor.r / 255.0, (float)baseColor.g / 255.0, (float)baseColor.g / 255.0, textureAlpha);
    
    for(auto itr = circles.begin(); itr != circles.end(); ++itr)
    {
        TJH_NameInTheStarsRenderer::Circle c = **itr;
        points.push_back(ofVec3f((float)c.x, (float)c.y, 0.0));
        sizes.push_back(ofVec3f(c.radius * 2 * textureWhitespaceScale));
        floatColors.push_back(col);
    }
    
}

void BrushWithGlowAndCirclePack::drawWithShader(ofShader shader)
{
    int total = (int)points.size();
    
//    ofVbo vbo;
    
    pointPositionsVbo.clear();
    pointPositionsVbo = ofVbo();
    pointPositionsVbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    pointPositionsVbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    pointPositionsVbo.setColorData(&floatColors[0], total, GL_STATIC_DRAW);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnablePointSprites();
    alphaChannelFbo.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(255, 255, 255);
    ofDrawRectangle(0, 0, 100, 100);
    shader.begin();
    pointTexture4Shader.bind();
    
    pointPositionsVbo.draw(GL_POINTS, 0, total);
    
    pointTexture4Shader.unbind();
    shader.end();
    alphaChannelFbo.end();
    
    if(hasGlow)
    {
        ofFbo blackPassFbo;
        blackPassFbo.allocate(_fboWidth, _fboHeight);
        
        blackPassFbo.begin();
        ofClear(0, 0, 0, 255);
        
        alphaChannelFbo.draw(0, 0);
        
        blackPassFbo.end();
        ofPixels pix;
        blackPassFbo.readToPixels(pix);
        blackPassImage = ofImage(pix);
        if(!blackBlurFbo.isAllocated())
        {
            blackBlurFbo.allocate(_fboWidth, _fboHeight);
        }
        blackBlurFbo.begin();
        blurFilter->begin();
        blackPassImage.draw(0, 0);
        blurFilter->end();
        blackBlurFbo.end();
    }
    
}

bool BrushWithGlowAndCirclePack::glowEnabled()
{
    return hasGlow;
}

void BrushWithGlowAndCirclePack::populateFromPixelsAndPaths(const ofPixels &pix, ofRectangle activeBounds, vector<ofPath> paths)
{
    circles.clear();
    sizes.clear();
    points.clear();
    floatColors.clear();
    
    _cirlceDensityMultiplier = ofMap((float)paths.size(), 3.0, 12.0, 0.95, 1.0);
    if(circleDensity > 0)
    {
        populateFromPixels(pix, activeBounds);
    }
    
    if(_insideShapeConstraint < 1.0)
    {
        
        ofVec3f rectPos = activeBounds.getBottomLeft();
        //draw some on the outside
        ofFloatColor col = ofFloatColor((float)baseColor.r / 255.0, (float)baseColor.g / 255.0, (float)baseColor.g / 255.0, textureAlpha);
        int pathCount = 0;
        for(auto pathItr = paths.begin(); pathItr != paths.end(); ++pathItr)
        {
            auto mesh = pathItr->getTessellation();
            auto vertices = mesh.getVertices();
            int k = (int)ofRandom(pathVertexSkip);
            int numVertices = vertices.size();
            for(int i = k; i<numVertices; i+=pathVertexSkip)
            {
                
                ofVec3f pos = mesh.getVertex(i);
                
                pos += rectPos;
                
                ofVec3f offset = ofVec3f(ofRandom(-brushOutlineScatterX, brushOutlineScatterX), ofRandom(-brushOutlineScatterY, brushOutlineScatterY), 0);
                pos += offset;
                //if it's outside (hence non color pixel) then sure, add a vertex vector
                if(pix.getColor((int)pos.x, (int)pos.y).r == 0)
                {
                    sizes.push_back(ofVec3f(ofRandom(minCircleSize, maxCircleSize) * textureWhitespaceScale * 2));
                    points.push_back(ofVec3f(pos.x, pos.y, 0.0));
                    floatColors.push_back(col);
                }
            }
        }
    }
    
    
}

int BrushWithGlowAndCirclePack::addCircles(int num, ofPixels pix, ofRectangle fromRect)
{
    float left = fromRect.getLeft();
    float right = fromRect.getRight();
    float top = fromRect.getTop();
    float bottom = fromRect.getBottom();
    int numAdded = 0;
    
    for(int i = 0; i<num; i++)
    {
        int x = (int)ofRandom(left, right);
        int y = (int)ofRandom(top, bottom);
        if(pix.getColor(x, y).r > 0)
        {
            bool overlap = false;
            for(int j = 0; j < circles.size(); j++){
                if(ofDist(x, y, circles[j]->x, circles[j]->y) <= circles[j]->radius + 2){
                    overlap = true;
                    break;
                }
            }
            if(!overlap){
                TJH_NameInTheStarsRenderer::Circle * c = new TJH_NameInTheStarsRenderer::Circle(minCircleSize, maxCircleSize, circleGrowthRate, baseColor);
                c->setup(x, y);
                circles.push_back(c);
                numAdded++;
            }
        }
    }
    
    return numAdded;
    
}