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

#ifndef BEBLOB_LOGIC_JOB_H
#define BEBLOB_LOGIC_JOB_H

#include <queue>
#include <memory>
#include <SFML/System/Vector2.hpp>

#include "common/unused.h"
#include "components/blob_solid.h"
#include "components/world.h"
#include "ai/astar.h"

namespace beblob
{
namespace utils
{

struct Job
{
    /// a job pointer type
    typedef std::shared_ptr<Job> Ptr;
    typedef enum
    {
        kNothing,
        kEating,
        kBuilding,
        kMoving,
        kDestroying
    } Action;

    Job(const sf::Vector2i& coor);

    virtual bool initiate(component::BlobSolid& physic,
                          component::World& world) = 0;

    virtual bool checkValidity(component::World& world) = 0;

    virtual bool collide(component::World& UNUSED(world)) {return false;}

    bool doStuff(double dt,
                 component::BlobSolid& physic,
                 component::World& world);

    virtual Action gameLoop(ecs::Entity& UNUSED(blob),
                          component::World& UNUSED(world),
                          ecs::EventManager::Ptr (events)) {return kNothing;}

    utils::AStar::Path getPath();
    sf::Vector2i getGoal() const;
protected:
    virtual bool process(double dt,
                         component::BlobSolid& physic,
                         component::World& world) = 0;
    bool onGoal(component::BlobSolid &physic) const;
    bool goodPath(component::World &world) const;
    void follow(component::BlobSolid &physic);
    AStar::Path path_;
    sf::Vector2i goal_;
    unsigned int current_;
    bool end_;
};

inline sf::Vector2i Job::getGoal() const
{
    return goal_;
}

typedef std::queue<Job::Ptr> Jobs;

}
}

#endif // BEBLOB_LOGIC_JOB_H
