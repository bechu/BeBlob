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

#include <cmath>

#include "math/vector.h"
#include "stick.h"

namespace beblob
{
namespace physic
{

Stick::Stick()
    : a(0)
    , b(0)
    , lengthSquared(0.0f)
{
}

Stick::Stick(PointMass& aA, PointMass& aB)
    : a(&aA)
    , b(&aB)
{
    float length = math::distance(a->currPos, b->currPos);
    lengthSquared = length * length;
}

Stick::Stick(const Stick& that)
    : a(that.a)
    , b(that.b)
{
    float length = math::distance(a->currPos, b->currPos);
    lengthSquared = length * length;
}

void Stick::scale(float scaleFactor)
{
    lengthSquared *= scaleFactor * scaleFactor;
}

void Stick::setLength(float length)
{
    lengthSquared = length * length;
}

void Stick::satisfy()
{
    sf::Vector2f delta(b->currPos);
    delta -= a->currPos;

    float scaleFactor;

    float dotProd = delta.x * delta.x + delta.y * delta.y;
    scaleFactor = lengthSquared / (dotProd + lengthSquared) - 0.5f;
    delta.x *= scaleFactor;
    delta.y *= scaleFactor;

    a->currPos -= delta;
    b->currPos += delta;
}

}
}
