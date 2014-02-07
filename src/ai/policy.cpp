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

#include "policy.h"
#include "math/vector.h"

namespace beblob
{
namespace utils
{

std::vector<sf::Vector2i> Team::getGoals()
{
    std::vector<sf::Vector2i> ret;
    for(auto m : *this)
        ret.push_back(m.goal);
    return ret;
}

void Team::sort_hungry()
{
    std::sort(begin(), end(), [](Member a, Member b) {
        return b.state < a.state;
    });
}


sf::Vector2i Policy::findTask(component::World& world,
                              const component::World::Cell::TypeList& types,
                              const sf::Vector2i& to,
                              const sf::Vector2i& from,
                              int radius_max,
                              bool direct)
{
    int radius = 0;
    sf::Vector2i res(-1, -1);
    do
    {
        res = findOnRadius(world, types, to, from, radius, direct);
        radius++;
        if(isGoal(res))
        {
            res = sf::Vector2i(-1, -1);
        }
        if(radius >= radius_max)
        {
            break;
        }
    } while(res.x == -1);
    return res;
}

namespace
{
bool testAround(component::World& world,
                const sf::Vector2i& start,
                const sf::Vector2i& loc)
{
    if(AStar(&world).search(start, sf::Vector2i(loc.x-1, loc.y)).size() > 0)
    {
        return true;
    }
    if(AStar(&world).search(start, sf::Vector2i(loc.x+1, loc.y)).size() > 0)
    {
        return true;
    }
    if(AStar(&world).search(start, sf::Vector2i(loc.x, loc.y-1)).size() > 0)
    {
        return true;
    }
    if(AStar(&world).search(start, sf::Vector2i(loc.x, loc.y+1)).size() > 0)
    {
        return true;
    }
    return false;
}
}

sf::Vector2i Policy::findOnRadius(component::World& world,
                                  const component::World::Cell::TypeList& types,
                                  const sf::Vector2i& to,
                                  const sf::Vector2i& from,
                                  int radius, bool direct)
{
    std::vector<int> candidate;
    for(int i=-radius;i<=radius;++i)
        candidate.push_back(i);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(candidate.begin(), candidate.end(), g);

    for(auto x : candidate)
    {
        for(auto y : candidate)
        {
            if(std::abs(x) == radius || std::abs(y) == radius)
            {
                sf::Vector2i loc(to.x+x, to.y+y);
                if(loc.x >= 0 && loc.y >= 0
                        && loc.x < component::World::kWidth
                        && loc.y < component::World::kHeight)
                {
                    if(std::find(types.begin(), types.end(), world(loc).type)
                            != types.end())
                    {
                        if(direct)
                        {
                            int size = AStar(&world).search(from, loc).size();
                            if(size > 0)
                                return loc;
                        }
                        else
                        {
                            if(testAround(world, from, loc))
                                return loc;
                        }
                    }
                }
            }
        }
    }
    return sf::Vector2i(-1, -1);
}

void Policy::computeGoals(utils::Team& team)
{
    goals_.clear();
    for(auto member : team)
        goals_.push_back(member.goal);
}

bool Policy::isGoal(const sf::Vector2i& goal) const
{
    if(goal == sf::Vector2i(-1, -1)) return false;
    return !(std::find(goals_.begin(), goals_.end(), goal) == goals_.end());
}

}
}
