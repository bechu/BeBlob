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

#include <algorithm>
#include <limits>
#include <SFML/OpenGL.hpp>

#include "math/random.h"
#include "math/vector.h"
#include "math/math.h"
#include "math/constants.h"
#include "blob_solid.h"

namespace beblob
{
namespace component
{

BlobSolid::BlobSolid(const sf::Vector2f& position) :
    middle_(position.x, position.y, 1.0f),
    radius_(BlobSolid::kDefaultRadius),
    computedMiddle_(position)
{
    uint32_t i = 0;
    for(i = 0; i < kNumPointMasses; i++)
    {
        float t = static_cast<float>(i) /
                static_cast<float>(kNumPointMasses) * PI * 2.0f;
        pointmasses_[i] =
                physic::PointMass(position.x + std::cos(t) * radius_,
                                  position.y + std::sin(t) * radius_, 1.0f);
    }

    // sticks_
    for(i = 1; i < kNumPointMasses; i++)
    {
        sticks_[i-1] = physic::Stick(pointmasses_[i-1], pointmasses_[i]);
    }

    sticks_[i-1] = physic::Stick(pointmasses_[0],
            pointmasses_[kNumPointMasses - 1]);

    float shortConst = 1.07f;
    float longConst = 1.45f;

    int j = 0;
    for(i = 0; i < kNumPointMasses; i++)
    {
        int t;


        int k = kNumPointMasses / 2;

        t = clampIndex(i - 5, kNumPointMasses);
        joints_[j] = physic::Joint(pointmasses_[i],
                                  pointmasses_[t], shortConst, longConst);
        j++;
        t = clampIndex(i + 5, kNumPointMasses);
        joints_[j] = physic::Joint(pointmasses_[i],
                                  pointmasses_[t], shortConst, longConst);
        j++;
        t = clampIndex(i + k - 6, kNumPointMasses);
        joints_[j] = physic::Joint(pointmasses_[i],
                                  pointmasses_[t], shortConst, longConst);
        j++;
        t = clampIndex(i + k + 6, kNumPointMasses);
        joints_[j] = physic::Joint(pointmasses_[i],
                                  pointmasses_[t], shortConst, longConst);
        j++;
        joints_[j] = physic::Joint(pointmasses_[i],
                                  middle_, shortConst, longConst);
        j++;
    }
}

void BlobSolid::scale(float factor)
{
    if(factor <= 0) return;

    for(auto& joint : joints_)
    {
        joint.scale(factor);
    }
    for(auto& stick : sticks_)
    {
        stick.scale(factor);
    }
    radius_ *= factor;
}

float BlobSolid::computeVolume() const
{
    float volume = 0.0f, t;

    for(unsigned int i = 0; i < sticks_.size(); i++)
    {
        const physic::Stick& stick = sticks_[i];

        t  = stick.begin()->currPos.x *
                (stick.end()->currPos.y - middle_.currPos.y);
        t += stick.end()->currPos.x *
                (middle_.currPos.y - stick.begin()->currPos.y);
        t += middle_.currPos.x *
                (stick.begin()->currPos.y - stick.end()->currPos.y);

        volume += fabs(t);
    }

    return volume * 0.5f;
}

void BlobSolid::computeMiddle(sf::Vector2f& aMiddle) const
{
    aMiddle.x = 0.0f;
    aMiddle.y = 0.0f;

    for(unsigned int i = 0; i < pointmasses_.size(); i++)
        aMiddle += pointmasses_[i].currPos;

    aMiddle.x *= 1.0f / static_cast<float>(kNumPointMasses);
    aMiddle.y *= 1.0f / static_cast<float>(kNumPointMasses);
}

void BlobSolid::pump()
{
    float targetVolume, actualVolume, scaleFactor;
    sf::Vector2f tMiddle;

    targetVolume = radius_ * radius_ * PI * 1.5f;
    actualVolume = computeVolume();

    if(actualVolume < targetVolume * 0.0001f)
        actualVolume = targetVolume * 0.0001f;

    scaleFactor = targetVolume / actualVolume;
    scaleFactor -= 1.0f;
    scaleFactor *= 0.1f;

    computeMiddle(tMiddle);

    for(unsigned int i = 0; i < pointmasses_.size(); i++)
    {
        sf::Vector2f t = pointmasses_[i].currPos;
        t -= tMiddle;
        t.x *= scaleFactor;
        t.y *= scaleFactor;
        pointmasses_[i].currPos += t;
    }
}

void BlobSolid::move(float dt)
{
    computeMiddle(computedMiddle_);
    for(size_t i = 0; i < pointmasses_.size(); i++)
    {
        pointmasses_[i].move(dt);
    }
    middle_.move(dt);
}


void BlobSolid::satisfy()
{
    for(int i = 0; i < 10; i++)
    {
        pump();

        for(auto& joint : joints_)
            joint.satisfy();

        for(auto& stick : sticks_)
            stick.satisfy();

        for(auto& pm : pointmasses_)
            pm.setFriction(0.01f);
    }
}

bool BlobSolid::simpleDistanceTest(const BlobSolid& b)
{
    float dist = math::distance_squared(middle_.currPos, b.middle_.currPos);
    float k = 1.5f * (getRadius() + b.getRadius());
    if(dist > k * k)
        return false;
    return true;
}

void BlobSolid::findSeparationPlane(const BlobSolid& b,
                                    const sf::Vector2f& aMiddle,
                                    sf::Vector2f& resultA,
                                    sf::Vector2f& resultB)
{
    float tx, ty, px, py;
    float k, kA, kB;
    sf::Vector2f bMiddle;

    /* M_PI constant removed, kA and kB are only valid for k computation */
    /* FIXME, get rid of sqrt by using one step taylor or something like that */
    kA = sqrt(computeVolume());
    kB = sqrt(b.computeVolume());
    k = kA / (kA + kB);

    b.computeMiddle(bMiddle);

    tx = bMiddle.x - aMiddle.x;
    ty = bMiddle.y - aMiddle.y;

    px = - ty;
    py = tx;

    /* reuse tx, ty */
    tx = tx * k + aMiddle.x;
    ty = ty * k + aMiddle.y;

    /* reuse px, py */
    px = px * 0.5f;
    py = py * 0.5f;

    resultA.x = tx + px;
    resultA.y = ty + py;

    resultB.x = tx - px;
    resultB.y = ty - py;
}

bool BlobSolid::privLineIntersect(const sf::Vector2f& lineA,
                                  const sf::Vector2f& lineB,
                                  const sf::Vector2f& segA,
                                  const sf::Vector2f& segB,
                                  sf::Vector2f& result)
{
    float s, k;

    /* the lines are paralle if k is zero */
    k = ((lineB.x-lineA.x) * (segB.y-segA.y) -
         (lineB.y-lineA.y) * (segB.x-segA.x));
    if(k == 0.0f)
        return false;

    s = ((lineA.y-segA.y) * (lineB.x-lineA.x) -
         (lineA.x-segA.x) * (lineB.y-lineA.y)) / k;
    if(s < 0.0f || s > 1.0f)
        return false;

    result = segB;
    result -= segA;
    result.x *= s;
    result.y *= s;
    result += segA;

    return true;
}

bool BlobSolid::distanceTest(const LandSolid& land)
{
    float dist = math::distance(middle_.currPos, land.getCenter());
    float k = 1.5f * getRadius();
    if(dist < k)
        return false;
    return true;
}

bool BlobSolid::collisionTest(const LandSolid& land, unsigned int i)
{
    if(distanceTest(land) == false) return false;
    sf::Vector2f tMiddle;
    computeMiddle(tMiddle);
    {
        physic::PointMass& p = pointmasses_[i];
        if(math::pnpoly(land.points, p.currPos))
        {
            std::vector<sf::Vector2f> a = land.points;
            std::reverse(std::begin(a), std::end(a));
            sf::Vector2f result;
            if(math::testcoll(land.points, tMiddle, p.currPos, result))
                p.currPos = result;
            p.setFriction(0.5f);
            return true;
        }
    }
    return false;
}

bool BlobSolid::isInside(const sf::FloatRect& zone)
{
    for(unsigned int i = 0; i < pointmasses_.size(); i++)
    {
        physic::PointMass& p = pointmasses_[i];
        if(zone.contains(p.currPos)) return true;
    }
    return false;
}

void BlobSolid::collisionTest(const Env& env)
{
    for(unsigned int i = 0; i < pointmasses_.size(); i++)
    {
        physic::PointMass& p = pointmasses_[i];
        if(env.collide(p.currPos))
            p.setFriction(0.5f);
    }
}

void BlobSolid::collisionTest(const LandSolid& land)
{
    if(distanceTest(land))
    {
        return ;
    }
    sf::Vector2f tMiddle;
    computeMiddle(tMiddle);
    for(unsigned int i = 0; i < pointmasses_.size(); i++)
    {
        physic::PointMass& p = pointmasses_[i];
        if(math::pnpoly(land.points, p.currPos))
        {
            sf::Vector2f result;
            if(math::testcoll(land.points, tMiddle, p.currPos, result))
            {
                p.currPos = result;
            }
            else
                p.currPos = p.prevPos;
            p.setFriction(0.5f);
        }
    }
}

void BlobSolid::collisionTest(const BlobSolid& another)
{
    if(!BlobSolid::simpleDistanceTest(another))
        return;

    sf::Vector2f tMiddle;
    computeMiddle(tMiddle);

    sf::Vector2f spA, spB;
    BlobSolid::findSeparationPlane(another, tMiddle, spA, spB);

    for(unsigned int i = 0; i < pointmasses_.size(); i++)
    {
        sf::Vector2f r;
        physic::PointMass& p = pointmasses_[i];
        if(BlobSolid::privLineIntersect(spA, spB, tMiddle, p.currPos, r))
        {
            p.currPos = r;
            p.setFriction(0.5f);
        }
    }
}

void BlobSolid::inverseForce()
{
    for(unsigned int i = 0; i < pointmasses_.size(); i++)
        pointmasses_[i].inverseForce();
    middle_.inverseForce();
}

void BlobSolid::setForce(const sf::Vector2f& force)
{
    for(unsigned int i = 0; i < pointmasses_.size(); i++)
        pointmasses_[i].setForce(force);
    middle_.setForce(force);
}

void BlobSolid::addForce(const sf::Vector2f& force)
{
    for(unsigned int i = 0; i < pointmasses_.size(); i++)
        pointmasses_[i].addForce(force);
    middle_.addForce(force);
}

bool BlobSolid::contains(const sf::Vector2f& point)
{
    std::vector<sf::Vector2f> points;
    for(unsigned int i = 0; i < pointmasses_.size(); i++)
    {
        points.push_back(pointmasses_[i].currPos);
    }
    sf::Vector2f notuse;
    return math::pnpoly(points, point);
}

bool BlobSolid::control(const sf::Vector2f& goal, float power)
{
    float deltaY = goal.y - getPosition().y;
    float deltaX = goal.x - getPosition().x;
    float angle = atan2(deltaY, deltaX);
    sf::Vector2f direction(std::cos(angle)*2, std::sin(angle)*2);
    setForce(direction*power);
    return math::distance<float>(goal, getPosition()) < LandSolid::kSize*.5f;
}

int BlobSolid::clampIndex(int index, int highBound) const
{
    while(index < 0)
        index = highBound + index;
    while(index >= highBound)
        index = index - highBound;
    return index;
}

void BlobSolid::collide(std::set<LandSolid::Ptr>& lands)
{
    std::queue<LandSolid::Ptr> remove;
    for(auto land : lands_)
    {
        bool found = std::find(lands.begin(), lands.end(), land) != lands.end();
        if(!found)
            remove.push(land);
    }
    while(remove.size() > 0)
    {
        for(auto it=lands_.begin();it!=lands_.end();++it)
        {
            if(*it == remove.front())
            {
                lands_.erase(it);
                break;
            }
        }
        remove.pop();
    }
}

bool BlobSolid::haveCollided(LandSolid::Ptr land)
{
    bool n = std::find(lands_.begin(), lands_.end(), land) == lands_.end();
    if(n) lands_.push_back(land);
    return n;
}

}
}
