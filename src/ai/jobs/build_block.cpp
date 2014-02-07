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

#include "build_block.h"
#include "common/events.h"

namespace beblob
{
namespace utils
{

BuildBlock::BuildBlock(const sf::Vector2i& coor)
    : Job(coor)
{
}

bool BuildBlock::initiate(component::BlobSolid& physic, component::World& world)
{
    path_ = AStar(&world).search(
                component::World::screen2map(physic.getPosition()),
                goal_);
    return static_cast<bool>(path_.size() > 0);
}

bool BuildBlock::checkValidity(component::World& world)
{
    return (world(goal_).type == component::World::Cell::kNothing
            || world(goal_).type == component::World::Cell::kFood);
}

Job::Action BuildBlock::gameLoop(ecs::Entity& blob,
                          component::World &world,
                          ecs::EventManager::Ptr events)
{
    if(state_ == Building)
    {
        auto P = blob.component<component::BlobSolid>();
        if(onGoal(*P))
        {
            world.setBuild(goal_, world(goal_).Build + 2);
            if(world(goal_).Build > 10)
            {
                state_ = BackOff;
                path_ = getClosest(world,
                                   component::World::screen2map(P->getPosition()),
                                   goal_);

                current_ = 0;
            }
            return utils::Job::kBuilding;
        }
    }
    else if(state_ == BackOff)
    {
     //   if(onGoal(physic))
        {
            state_ = WaitNobody;
            end_ = true;
            events->emit<event::World::AddBlock>(goal_);
        }
    }
    return utils::Job::kNothing;
}

bool BuildBlock::process(double dt,
                         component::BlobSolid& physic,
                         component::World& world)
{
    static sf::Vector2f last;
    static double timeout = 0;
    if(last == physic.getPosition())
    {
        timeout += dt;
        if(timeout >= 1000)
        {
            timeout = 0;
            physic.inverseForce();
        }
    }
    last = physic.getPosition();
    follow(physic);
    return !end_;
}

}
}
