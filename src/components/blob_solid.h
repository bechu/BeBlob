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

#ifndef BEBLOB_PHYSICS_BLOB_H
#define BEBLOB_PHYSICS_BLOB_H

#include <array>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include "physic/stick.h"
#include "physic/joint.h"
#include "physic/pointmass.h"
#include "ecs/ecs.h"
#include "land_physic.h"
#include "math/vector.h"
#include "env.h"

namespace beblob
{
namespace component
{

////////////////////////////////////////////////////////////
/// \brief a physic blob definition
///
/// \see http://www.blobsallad.se/
////////////////////////////////////////////////////////////
class BlobSolid
        : public ecs::Component<BlobSolid>
{
public:
    constexpr static unsigned int kNumPointMasses = 40;
    constexpr static unsigned int kDefaultRadius = 20;

    ////////////////////////////////////////////////////////////
    /// \brief create a blob
    ///
    /// \param position current position of the blob
    ////////////////////////////////////////////////////////////
    BlobSolid(const sf::Vector2f& position);

    ////////////////////////////////////////////////////////////
    /// \brief scale the blob
    ///
    /// \param factor the scale factor
    ///
    /// \example scale(1.1f);
    ////////////////////////////////////////////////////////////
    void scale(float factor);

    ////////////////////////////////////////////////////////////
    /// \brief a blob controller, apply force to aim the goal
    ///
    /// \param goal the aim
    /// \param power the powerness
    ///
    ////////////////////////////////////////////////////////////
    bool control(const sf::Vector2f& goal, float power=1);

    ////////////////////////////////////////////////////////////
    /// \brief an blob actuator, update the position
    ///
    /// \param dt delta time
    ////////////////////////////////////////////////////////////
    void move(float dt);

    ////////////////////////////////////////////////////////////
    /// \brief apply the physic
    ////////////////////////////////////////////////////////////
    void satisfy();

    ////////////////////////////////////////////////////////////
    /// \brief check if point is inside the blob
    ///
    /// \param point the test point
    ///
    /// \return true if the point is inside
    ////////////////////////////////////////////////////////////
    bool contains(const sf::Vector2f& point);

    ////////////////////////////////////////////////////////////
    /// \brief test if the blob is in a rect
    ///
    /// \param zone the test zone
    ///
    /// \return true if the blob is fully inside
    ////////////////////////////////////////////////////////////
    bool isInside(const sf::FloatRect& zone);

    ////////////////////////////////////////////////////////////
    /// \brief have already collide ?
    ///
    /// \param land land to test
    ///
    /// \return true if collision
    ////////////////////////////////////////////////////////////
    bool haveCollided(LandSolid::Ptr land);

    ////////////////////////////////////////////////////////////
    /// \brief collision test with a list of land
    ///
    /// \param land list of lands
    ///
    /// \return true if collision
    ////////////////////////////////////////////////////////////
    void collide(std::set<LandSolid::Ptr>& lands);

    ////////////////////////////////////////////////////////////
    /// \brief test a collision with blob solid
    ///
    /// \param another another solid blob
    ////////////////////////////////////////////////////////////
    void collisionTest(const BlobSolid& another);

    ////////////////////////////////////////////////////////////
    /// \brief collision test with a list of land
    ///
    /// \param land
    /// \param i specific mass point
    ///
    /// \return true if collision
    ////////////////////////////////////////////////////////////
    bool collisionTest(const LandSolid& land, unsigned int i);

    ////////////////////////////////////////////////////////////
    /// \brief test a collision with land solid
    ///
    /// \param land
    ////////////////////////////////////////////////////////////
    void collisionTest(const LandSolid& land);

    ////////////////////////////////////////////////////////////
    /// \brief test a collision with the environment
    ///
    /// \param env
    ////////////////////////////////////////////////////////////
    void collisionTest(const Env& env);

    ////////////////////////////////////////////////////////////
    /// \brief set force
    ///
    /// \param force
    ////////////////////////////////////////////////////////////
    void setForce(const sf::Vector2f& force);

    ////////////////////////////////////////////////////////////
    /// \brief add force
    ///
    /// \param force
    ////////////////////////////////////////////////////////////
    void addForce(const sf::Vector2f& force);

    ////////////////////////////////////////////////////////////
    /// \brief inverse the force (the direction)
    ////////////////////////////////////////////////////////////
    void inverseForce();

    physic::PointMass& getPointMasse(uint32_t index);

    ////////////////////////////////////////////////////////////
    /// \brief return the position of the blob
    ///
    /// \param 2d position
    ////////////////////////////////////////////////////////////
    sf::Vector2f getPosition() const;

    ////////////////////////////////////////////////////////////
    /// \brief return the middle mass point
    ///
    /// \return middle point
    ////////////////////////////////////////////////////////////
    physic::PointMass& getMiddle();

    ////////////////////////////////////////////////////////////
    /// \brief set the radius value
    ///
    /// \param r new radisu
    ////////////////////////////////////////////////////////////
    void setRadius(float r);

    ////////////////////////////////////////////////////////////
    /// \brief get the radius
    ///
    /// \param float radius
    ////////////////////////////////////////////////////////////
    float getRadius() const;
private:
    ////////////////////////////////////////////////////////////
    /// \brief compute the middle point of the blob
    ///
    /// \param aMiddle the calculated point
    ////////////////////////////////////////////////////////////
    void computeMiddle(sf::Vector2f& aMiddle) const;

    ////////////////////////////////////////////////////////////
    /// \brief compute the volume
    ///
    /// \return the area
    ////////////////////////////////////////////////////////////
    float computeVolume() const;

    ////////////////////////////////////////////////////////////
    /// \brief test the distance with another blob
    ///
    /// \return true if the blob is close
    ////////////////////////////////////////////////////////////
    bool simpleDistanceTest(const BlobSolid& b);

    ////////////////////////////////////////////////////////////
    /// \brief find the separtion place
    ///
    /// \param b  blob
    /// \param aMiddle
    /// \param resultA
    /// \param resultB
    ////////////////////////////////////////////////////////////
    void findSeparationPlane(const BlobSolid& b,
                                 const sf::Vector2f& aMiddle,
                                 sf::Vector2f& resultA,
                                 sf::Vector2f& resultB);

    ////////////////////////////////////////////////////////////
    /// \brief intersection test
    ////////////////////////////////////////////////////////////
    bool privLineIntersect(const sf::Vector2f &lineA,
                           const sf::Vector2f &lineB,
                           const sf::Vector2f &segA,
                           const sf::Vector2f &segB,
                           sf::Vector2f& result);

    ////////////////////////////////////////////////////////////
    /// \brief test the distance with a land
    ///
    /// \param land a land
    ///
    /// \return bool true if close
    ////////////////////////////////////////////////////////////
    bool distanceTest(const LandSolid& land);

    ////////////////////////////////////////////////////////////
    /// \brief clamp a value beetween 0 to highBound with an offset
    ///
    /// \param index the intial value to test
    /// \param highBound max value of the return value
    ///        used like an offset
    ///
    /// \return the fixed index
    ////////////////////////////////////////////////////////////
    int clampIndex(int index, int highBound) const;

    ////////////////////////////////////////////////////////////
    /// \brief pump
    ////////////////////////////////////////////////////////////
    void pump();

    /// middle mass point
    physic::PointMass middle_;
    /// radius of the blob
    float radius_;
    /// middle point
    sf::Vector2f computedMiddle_;
    /// sticks ...
    std::array<physic::Stick, kNumPointMasses> sticks_;
    /// mass points
    std::array<physic::PointMass, kNumPointMasses> pointmasses_;
    /// joints !
    std::array<physic::Joint, kNumPointMasses*5> joints_;
    /// true if blob collide
    bool collision_ = false;
    /// list of collided lands
    std::vector<LandSolid::Ptr> lands_;
};

inline sf::Vector2f BlobSolid::getPosition() const
{
    return middle_.currPos;
}

inline float BlobSolid::getRadius() const
{
    return radius_;
}

inline void BlobSolid::setRadius(float r)
{
    radius_ = r;
}

inline physic::PointMass& BlobSolid::getMiddle()
{
    return middle_;
}

inline physic::PointMass& BlobSolid::getPointMasse(uint32_t index)
{
    assert(index < pointmasses_.size());
    return pointmasses_[index];
}

}
}

#endif // BEBLOB_PHYSICS_BLOB_H

