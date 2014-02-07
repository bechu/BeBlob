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

#ifndef BEBLOB_LOGIC_BEST_WAY_H
#define BEBLOB_LOGIC_BEST_WAY_H

#include "astar.h"

namespace beblob
{
namespace utils
{

////////////////////////////////////////////////////////////
/// \brief find the shortest path from start
///        to around (+/- 1) end
///        (based on astar)
////////////////////////////////////////////////////////////
class BestWay
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief return the closest path to aim the goal
    ///        test the four candidate around the goal
    ///
    /// \param world the world used to compute the best path
    /// \param start beginning of the path
    /// \param end goal of the future movement
    ///
    /// \return the closest path to go to the goal
    ///         if no path is possible return an empy path
    ////////////////////////////////////////////////////////////
    AStar::Path getClosest(component::World& world,
                           const sf::Vector2i& start,
                           const sf::Vector2i& end);
private:
    ////////////////////////////////////////////////////////////
    /// \brief return the path with an specify goal offset
    ///
    /// \param world used to compute path
    /// \param start beginning of the path
    /// \param end goal of the future path
    /// \param xoffset x goal offset
    /// \param yoffset y goal offset
    ///
    /// \return the path frm start to end + offset
    ///         if it's not possible return an empty path
    ////////////////////////////////////////////////////////////
    AStar::Path testCandidate(component::World& world,
                            const sf::Vector2i& start,
                            const sf::Vector2i& end,
                            int xoffset,
                            int yoffset);
};

}
}

#endif // BEBLOB_LOGIC_BEST_WAY_H
