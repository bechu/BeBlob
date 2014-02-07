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

#include "eat_food.h"
#include "components/blob_solid.h"
#include "components/blob_ai.h"

namespace beblob
{
namespace utils
{

EatFood::EatFood(const sf::Vector2i& coor)
    : Job(coor)
{
}

bool EatFood::initiate(component::BlobSolid& physic, component::World& world)
{
    path_ = AStar(&world).search(
                component::World::screen2map(physic.getPosition()),
                goal_);
    begin_eat_ = false;
    return static_cast<bool>(path_.size() > 0);
}

bool EatFood::checkValidity(component::World& world)
{
    return world(goal_).type == component::World::Cell::kFood;
}

bool EatFood::process(double dt,
                      component::BlobSolid& physic,
                      component::World& world)
{
    follow(physic);
    return !end_;
}

Job::Action EatFood::gameLoop(ecs::Entity& blob,
                              component::World& world,
                              ecs::EventManager::Ptr UNUSED(events))
{
    if(onGoal(*blob.component<component::BlobSolid>()))
    {
        blob.component<component::BlobAI>()->eat();
        world.setFood(goal_, world(goal_).Food - 5);
        if(world(goal_).Food < 0)
        {
            world.setFood(goal_, 0);
            world.setType(goal_, component::World::Cell::kNothing);
            end_ = true;
        }
        if(!begin_eat_)
        {
            begin_eat_ = true;
            return kEating;
        }
    }
    return kNothing;
}

}
}
