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

#include "astar.h"
#include "math/vector.h"

namespace beblob
{
namespace utils
{

AStar::AStar(const component::World *world)
{
    world_ = world;
}

AStar::Path AStar::search(const sf::Vector2i& start, const sf::Vector2i& end)
{
    start_ = start;
    goal_ = end;

    // special case : the goal is just close to the start
    if((abs(start.x-end.x) == 1 && start.y == end.y)
            || (abs(start.y-end.y) == 1 && start.x == end.x)
            || (start.y == end.y && start.x == end.x))
    {
        Path path;
        path.push_back(end);
        return path;
    }

    Node n;
    // current node is start
    sf::Vector2i current = start;
    // in the open list i add a node
    open_[current] = n;

    while(current != end && open_.empty() == false)
    {
        current = bestNode(open_);
        sinkOpenToClose(current);
        grabAdjacent(current);
    }

    if(current != end)
    {
        return Path();
    }

    return getPath();
}

AStar::Path AStar::getPath()
{
    Path path;

    sf::Vector2i current = goal_;
    do
    {
        Node &node = close_[current];
        path.push_front(current);
        current = node.parent;
    } while(current != start_);

    return path;
}

void AStar::sinkOpenToClose(const sf::Vector2i &loc)
{
    Node &node = open_[loc];
    close_[loc] = node;
    open_.erase(loc);
}

bool AStar::outOfWorld(const sf::Vector2i& loc)
{
    return !(loc.x >= 0
            && loc.y >= 0
            && loc.x < component::World::kWidth
            && loc.y < component::World::kHeight);
}

bool AStar::grabAdjacent(const sf::Vector2i& n)
{
    bool ret = false;
    for (int i=n.x-1; i<=n.x+1;++i)
    {
        for(int k=n.y-1;k<=n.y+1;++k)
        {
            sf::Vector2i it(i, k);
            if(outOfWorld(it)) continue ;
            if(it == n) continue;

            // in my game i can't move in diagonale !
            if(std::abs(i-n.x) == 1 && std::abs(k-n.y) == 1) continue;

            if((*world_)(it).isCollisionable() == false)
            {
                if(close_.find(it) == close_.end())
                {
                    ret = true;
                    Node tmp;
                    tmp.g = close_[n].g + math::distance<int>(it, n);
                    tmp.h = math::distance(it, goal_);
                    tmp.f = tmp.g + tmp.h;
                    tmp.parent = n;
                    if(open_.find(it) != open_.end())
                    {

                        if (tmp.f < open_[it].f)
                        {
                            open_[it] = tmp;
                        }
                    }
                    else
                    {
                        open_[it] = tmp;
                    }
                }
            }
        }
    }
    return ret;
}

sf::Vector2i AStar::bestNode(std::map<sf::Vector2i, Node, NodeLessThan> &list)
{
    float cost = list.begin()->second.f;
    sf::Vector2i ret = list.begin()->first;
    for (auto i = list.begin();i!=list.end(); ++i)
    {
        if (i->second.f < cost)
        {
            cost = i->second.f;
            ret = i->first;
        }
    }
    return ret;
}

}
}
