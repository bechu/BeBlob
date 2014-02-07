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

#ifndef BEBLOB_COMPONENT_LAND_SOLID_H
#define BEBLOB_COMPONENT_LAND_SOLID_H

#include <SFML/System/Vector2.hpp>

#include "ecs/component.h"
#include "physic/quadtree.h"

namespace beblob
{
namespace component
{

class LandSolid
        : public physic::QuadTree::Object
{
public:
    /// this is the size for one land (square)
    constexpr static int kSize = 60;

    /// this is the pointer shared definition of a solid land
    typedef std::shared_ptr<LandSolid> Ptr;

    ////////////////////////////////////////////////////////////
    /// \brief Land solid constructor
    ////////////////////////////////////////////////////////////
    LandSolid()
        : physic::QuadTree::Object(0, 0, 1, 1)
    {
    }

    sf::Vector2f getCenter() const;

    std::vector<sf::Vector2f> points;

    float max_ = 0;

    inline bool contains(const sf::Vector2i& point)
    {
        return (point.x >= Min.x && point.y >= Min.y
                && point.x <= Max.x && point.y <= Max.y);
    }

    sf::Vector2i Min;
    sf::Vector2i Max;
};

inline sf::Vector2f LandSolid::getCenter() const
{
    return sf::Vector2f(x+width/2, y+height/2);
}

}
}

#endif // BEBLOB_COMPONENT_LAND_SOLID_H

