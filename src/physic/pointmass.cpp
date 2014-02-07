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

#include "pointmass.h"

namespace beblob
{
namespace physic
{

PointMass::PointMass() :
    currPos(0.0f, 0.0f),
    prevPos(0.0f, 0.0f),
    force_(0.0f, 0.0f),
    mass_(0.0f),
    friction_(0.0f)
{
}

PointMass::PointMass(float x, float y, float mass) :
    currPos(x, y),
    prevPos(x, y),
    force_(0.0f, 0.0f),
    mass_(mass),
    friction_(0.99f)
{
}

PointMass::PointMass(const PointMass& that) :
    currPos(that.currPos),
    prevPos(that.prevPos),
    force_(that.force_),
    mass_(that.mass_),
    friction_(that.friction_)
{
}

void PointMass::move(float dt)
{
    float t, a, c, dtdt, f1, f2;

    dtdt = dt * dt;
    f1 = friction_;
    f2 = 1.0f + f1;

    a = force_.x / mass_;
    c = currPos.x;
    t = f2 * c - f1 * prevPos.x + a * dtdt;
    prevPos.x = c;
    currPos.x = t;

    a = force_.y / mass_;
    c = currPos.y;
    t = f2 * c - f1 * prevPos.y + a * dtdt;
    prevPos.y = c;
    currPos.y = t;
}

void PointMass::setFriction(float friction)
{
    friction_ = 1.0f - friction;
}

void PointMass::setForce(const sf::Vector2f& force)
{
    force_ = force;
}

void PointMass::addForce(const sf::Vector2f& force)
{
    force_ += force;
}

void PointMass::inverseForce()
{
    force_.x = force_.x * -0.5;
    force_.y = force_.y * -0.5;
}

}
}
