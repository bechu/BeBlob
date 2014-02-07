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

#include "job.h"

namespace beblob
{
namespace utils
{

Job::Job(const sf::Vector2i& coor) :
    goal_(coor),
    current_(0),
    end_(false)
{
}

void Job::follow(component::BlobSolid& physic)
{
    if(physic.control(component::World::map2screen(path_[current_])))
    {
        if(current_ < path_.size()-1)
            current_++;
    }
}

bool Job::onGoal(component::BlobSolid& physic) const
{
    return component::World::screen2map(physic.getPosition()) == goal_;
}

bool Job::doStuff(double dt, component::BlobSolid& physic, component::World& world)
{
    // sure that the initial condition is already true
    if(checkValidity(world) == false)
    {
        end_ = true;
        return false;
    }
    // ok but the path is already usable ?
    if(onGoal(physic) == false)
    {
        if(goodPath(world) == false)
        {
            end_ = true;
            return false;
        }
    }
    // well continue to do the job
    return process(dt, physic, world);
}

bool Job::goodPath(component::World& world) const
{
    for(auto part : path_)
    {
        if(world(part).isCollisionable() == true)
            return false;
    }
    return true;
}

AStar::Path Job::getPath()
{
    return path_;
}

}
}
