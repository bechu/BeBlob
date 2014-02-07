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

#include "goto.h"

namespace beblob
{
namespace utils
{

GoTo::GoTo(const sf::Vector2i& coor)
    : Job(coor)
{
}

bool GoTo::initiate(component::BlobSolid& physic, component::World& world)
{
    path_ = AStar(&world).search(
                component::World::screen2map(physic.getPosition()),
                goal_);
    return static_cast<bool>(path_.size() > 0);
}

bool GoTo::checkValidity(component::World& world)
{
    return world(goal_).isCollisionable() == false;
}

bool GoTo::process(double dt,
                   component::BlobSolid& physic,
                   component::World& world)
{
    follow(physic);
    return !onGoal(physic);
}

Job::Action GoTo::gameLoop(ecs::Entity& blob,
                  component::World& world,
                  ecs::EventManager::Ptr events)
{
    return kMoving;
}

}
}
