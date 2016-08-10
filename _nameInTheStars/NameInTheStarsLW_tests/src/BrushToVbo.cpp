//
//  BrushToVbo.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 15/06/2016.
//
//

#include "BrushToVbo.hpp"

BrushToVbo::BrushToVbo(ofTexture pointTextureRef, float width, float height)
{
    spreadX = 0;
    spreadY = 0;
    spreadZ = 0;
    maxFailCount = 10.0f;
    insideShapeConstraint = 0.0f;
    repeatIterations = 1;
    vertexSkip = 1;
    blendMode = OF_BLENDMODE_DISABLED;
    alpha = 1.0;
    minSize = 10;
    maxSize = 50;
    size = ofRandom(minSize, maxSize);
    baseColor = ofColor(255, 228, 165);
    
    pointTexture = pointTextureRef;
    
    alphaChannelFbo.allocate(width, height, GL_RGBA);
    
}

ofTexture BrushToVbo::getTexture()
{
    return pointTexture;
}

void BrushToVbo::addPoint(float x, float y, float z, ofPath *path)
{
    
    path->close();
    
    auto outline = path->getOutline();
    ofVec3f v(x, y, z);
    
    
    ofVec3f offset(ofRandom(-spreadX, spreadX), ofRandom(-spreadY, spreadY), ofRandom(-spreadZ, spreadZ));
    ofVec3f pos = v + offset;
    bool valid = true;
    
    if(insideShapeConstraint > 0.0)
    {
        auto rollDice = ofRandom(0.0, 1.0);
        bool forceInside = rollDice < insideShapeConstraint;
        auto pointInside = find_if(outline.begin(), outline.end(), [=](ofPolyline pLine){
            return pLine.inside(pos.x, pos.y);
        });
    
        int failCount = 0;
        
    
        while(forceInside && pointInside != outline.end())
        {
            offset = ofVec3f(ofRandom(-spreadX, spreadX), ofRandom(-spreadY, spreadY), ofRandom(-spreadZ, spreadZ));
            pos = v + offset;
            pointInside = find_if(outline.begin(), outline.end(), [=](ofPolyline pLine){
                return pLine.inside(pos.x, pos.y);
            });
            
            forceInside = ++failCount < maxFailCount;
            ofLog() << " force attempt, failCount " << failCount;
        }
        
        valid = pointInside != outline.end();
        
    }
    
    valid = true;//hack
    
    if(valid)
    {
        points.push_back(pos);
        float pSize = ofRandom(minSize, maxSize);
        sizes.push_back(ofVec3f(pSize));
        ofFloatColor col = ofFloatColor((float)baseColor.r / 255, (float)baseColor.g / 255, (float)baseColor.b / 255, alpha);
        colours.push_back(col);
    }
    
    
    
}

void BrushToVbo::populateFromMesh(ofMesh *mesh, ofPath *path)
{
    auto const vertices = mesh->getVertices();
    points.clear();
    sizes.clear();
    colours.clear();
    int k = vertexSkip < vertices.size() ? int(ofRandom(0, vertexSkip)) : 0;
    for(int i = k; i<vertices.size(); i+= vertexSkip)
    {
        ofVec3f v = vertices[i];
        for(int j = 0; j<repeatIterations; j++)
        {
            addPoint(v.x, v.y, v.z, path);
        }
    }
}

BrushToVbo::~BrushToVbo()
{
    if(pointTexture.isAllocated())
    {
        pointTexture.clear();
    }
}