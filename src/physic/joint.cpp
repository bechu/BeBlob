/////////////////////////////////////////////////////////////////////////
/// BeBlob
/// Copyright (C) 2014 Jérôme Béchu
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "joint.h"

namespace beblob
{
namespace physic
{

Joint::Joint()
    : a(0)
    , b(0)
    , shortConstSquared(0.0f)
    , longConstSquared(0.0f)
{
}

Joint::Joint(PointMass& aA, PointMass& aB, float aShortConst, float aLongConst)
    : a(&aA)
    , b(&aB)
{
    sf::Vector2f delta(b->currPos);
    delta -= a->currPos;

    float dotProd = delta.x * delta.x + delta.y * delta.y;

    shortConstSquared = dotProd * aShortConst * aShortConst;
    longConstSquared = dotProd * aLongConst * aLongConst;
}

void Joint::scale(float scaleFactor)
{
    float sf2 = scaleFactor * scaleFactor;
    shortConstSquared *= sf2;
    longConstSquared *= sf2;
}

void Joint::setConstraints(float aShortConst, float aLongConst)
{
    shortConstSquared = aShortConst * aShortConst;
    longConstSquared = aLongConst * aLongConst;
}

void Joint::satisfy()
{
    sf::Vector2f delta(b->currPos);
    delta -= a->currPos;

    float scaleFactor;
    float dotProd = delta.x * delta.x + delta.y * delta.y;

    if(dotProd < shortConstSquared)
    {
        scaleFactor = shortConstSquared / (dotProd + shortConstSquared) - 0.5f;
        delta.x *= scaleFactor;
        delta.y *= scaleFactor;
        a->currPos -= delta;
        b->currPos += delta;
    }
    else if(dotProd > longConstSquared)
    {
        scaleFactor = longConstSquared / (dotProd + longConstSquared) - 0.5f;
        delta.x *= scaleFactor;
        delta.y *= scaleFactor;
        a->currPos -= delta;
        b->currPos += delta;
    }
}

}
}
