//
//  GiorgiaStars3.cpp
//  NameInTheStarsLW_tests
//
//  Created by Liam Walsh on 02/06/2016.
//
//

#include "GiorgiaStars3.hpp"


void GiorgiaStars3::setup(float width, float height)
{
    quadWarper.setSize(width, height);
    
}

void GiorgiaStars3::update(const string childName, const string fontName)  {
    _childName = childName;
    font.load(fontName, 400);
}

