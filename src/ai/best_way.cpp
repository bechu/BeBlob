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

#include "best_way.h"
#include "math/vector.h"

namespace beblob
{
namespace utils
{

AStar::Path BestWay::testCandidate(component::World& world,
                                   const sf::Vector2i& start,
                                   const sf::Vector2i& end,
                                   int xoffset,
                                   int yoffset)
{
    sf::Vector2i free = sf::Vector2i(end.x+xoffset, end.y+yoffset);
    if(world(free).isCollisionable() == false)
    {
        return AStar(&world).search(start, free);
    }
    return AStar::Path();
}

AStar::Path BestWay::getClosest(component::World& world, const sf::Vector2i& start, const sf::Vector2i& end)
{
    AStar::Path path;
    AStar::Path dummy;
    dummy = path = testCandidate(world, start, end, -1, 0);
    dummy = testCandidate(world, start, end, 0, -1);
    if((dummy.size() < path.size() && dummy.size() > 0)
            || path.size() == 0)
        path = dummy;
    dummy = testCandidate(world, start, end, 1, 0);
    if((dummy.size() < path.size() && dummy.size() > 0)
            || path.size() == 0)
        path = dummy;
    dummy = testCandidate(world, start, end, 0, 1);
    if((dummy.size() < path.size() && dummy.size() > 0)
            || path.size() == 0)
        path = dummy;
    return path;
}

}
}
