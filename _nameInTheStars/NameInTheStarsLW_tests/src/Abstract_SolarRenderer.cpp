//
//  Abstract_SolarRenderer.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 29/04/2016.
//
//

#include "Abstract_SolarRenderer.hpp"


void AbstractSolarRenderer::draw() {
    ofLog() << "abstract draw";
}

void AbstractSolarRenderer::drawDebug() {
    if(!_debugModeEnabled)
    {
        return;
    }
}

void AbstractSolarRenderer::setup(float width, float height)
{
    //
}
void AbstractSolarRenderer::update(const string childName, const string fontName)
{
    setName(childName);
}
void AbstractSolarRenderer::dispose()
{
    
}
void AbstractSolarRenderer::setName(const string name)
{
    _childName = name;
}
void AbstractSolarRenderer::setFont(const string fontName)
{
    
}
const bool AbstractSolarRenderer::getDebugModeEnabled() {
    return _debugModeEnabled;
}

void AbstractSolarRenderer::setDebugModeEnabled(bool enabled) {
    _debugModeEnabled = enabled;
}