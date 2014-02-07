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

#ifndef BEBLOB_LOGIC_ASTAR_H
#define BEBLOB_LOGIC_ASTAR_H

#include <SFML/System/Vector2.hpp>
#include <utility>
#include <map>

#include "components/world.h"

namespace beblob
{
namespace utils
{

////////////////////////////////////////////////////////////
/// \brief the astar implementation
////////////////////////////////////////////////////////////
class AStar
{
public:
    /// path definition (collection of coordinates)
    typedef std::deque<sf::Vector2i> Path;
    /// node definition
    struct Node
    {
        Node()
            : g(0)
            , h(0)
            , f(0)
        {}
        float g, h, f;
        sf::Vector2i parent;
    };

    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ///
    /// \param world take a ref on a world
    ////////////////////////////////////////////////////////////
    AStar(const component::World* world);

    ////////////////////////////////////////////////////////////
    /// \brief search the best way between two coordinate
    ///
    /// \param start where you start
    /// \param end the goal
    ///
    /// \return path a list of coordinate, in case of failed
    ///     return a path with no occurence
    ////////////////////////////////////////////////////////////
    Path search(const sf::Vector2i& start, const sf::Vector2i& end);
private:
    /// NodeLess use to compare two nodes
    struct NodeLessThan
            : public std::binary_function<sf::Vector2i, sf::Vector2i, bool>
    {
        bool operator() (sf::Vector2i const& n1, sf::Vector2i const& n2) const
        {
            return 	(n1.x<n2.x && n1.x != n2.x) ||
                    (n1.x == n2.x && n1.y<n2.y && n1.y != n2.y);
        }
    };

    ////////////////////////////////////////////////////////////
    /// \brief return the list of coordinate for the best path
    ///
    /// \return path
    ////////////////////////////////////////////////////////////
    Path getPath();

    bool grabAdjacent(const sf::Vector2i& loc);

    ////////////////////////////////////////////////////////////
    /// \brief test if the location is out of the world
    ///
    /// \param loc the location have to test
    ///
    /// \return true if is inside
    ////////////////////////////////////////////////////////////
    bool outOfWorld(const sf::Vector2i& loc);

    ////////////////////////////////////////////////////////////
    /// \brief return the best node from the list
    ///         take the node with the smallest cost value
    ///
    /// \param list list of node and coordinate
    ///
    /// \return the best coordinate
    ////////////////////////////////////////////////////////////
    sf::Vector2i bestNode(std::map<sf::Vector2i, Node, NodeLessThan> &list);

    ////////////////////////////////////////////////////////////
    /// \brief take data from open list and fill close list
    ///
    /// \param loc the location concerned
    ////////////////////////////////////////////////////////////
    void sinkOpenToClose(const sf::Vector2i &loc);

    /// the list of open position
    std::map<sf::Vector2i, Node, NodeLessThan> open_;
    /// the list of close position
    std::map<sf::Vector2i, Node, NodeLessThan> close_;
    /// the goal of AStar
    sf::Vector2i goal_;
    /// the start of AStar
    sf::Vector2i start_;
    /// pointer to the world
    const component::World* world_;
};

}
}

#endif // BEBLOB_LOGIC_ASTAR_H
