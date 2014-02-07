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

#include "math/vector.h"
#include "break_block.h"

namespace beblob
{
namespace utils
{

BreakBlock::BreakBlock(const sf::Vector2i& coor) :
    Job(coor),
    state_(BreakBlock::Idle),
    timeout_(0)
{
}

bool BreakBlock::initiate(component::BlobSolid& physic, component::World& world)
{
    path_ = getClosest(world,
                       component::World::screen2map(physic.getPosition()),
                       goal_);
    state_ = BreakBlock::GoTo;
    timeout_ = 100;
    return static_cast<bool>(path_.size() > 0);
}

bool BreakBlock::checkValidity(component::World& world)
{
    return world(goal_).type == component::World::Cell::kWall
            || world(goal_).type == component::World::Cell::kHome;
}

bool BreakBlock::collide(component::World& world)
{
    world.setResistance(goal_, world(goal_).Resistance-1);
    if(world(goal_).Resistance < 0)
    {
        world.setType(goal_, component::World::Cell::kNothing);
        end_ = true;
    }
    return end_;
}

bool BreakBlock::process(double dt,
                         component::BlobSolid& physic,
                         component::World& world)
{
    if(state_ == BreakBlock::Idle)
    {
        physic.setForce(sf::Vector2f());
    }
    if(state_ == BreakBlock::GoTo)
    {
        follow(physic);
        if(math::distance<float>(component::World::map2screen(goal_), physic.getPosition())
                < component::LandSolid::kSize)
        {
            state_ = BreakBlock::Breaking;
            timeout_ = 100;
        }
    }
    else if(state_ == BreakBlock::Breaking)
    {
        physic.control(component::World::map2screen(goal_), 2);
        if(timeout_ < 0)
        {
            timeout_ = 400;
            state_ = BreakBlock::Backward;
            physic.inverseForce();
        }
    }
    else if(state_ == BreakBlock::Backward)
    {
        if(timeout_ < 0)
        {
            timeout_ = 1000;
            state_ = BreakBlock::Breaking;
        }
    }
    timeout_-=dt;
    return !end_;
}

}
}
