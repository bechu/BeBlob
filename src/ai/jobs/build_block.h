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

#ifndef BEBLOB_LOGIC_BUILD_BLOCK_H
#define BEBLOB_LOGIC_BUILD_BLOCK_H

#include "ai/job.h"
#include "ai/best_way.h"

namespace beblob
{
namespace utils
{

class BuildBlock
        : public Job
        , public BestWay
{
public:
    BuildBlock(const sf::Vector2i& coor);
    bool initiate(component::BlobSolid& physic, component::World& world);
    bool checkValidity(component::World& world);
    Job::Action gameLoop(ecs::Entity& blob,
                  component::World &world,
                  ecs::EventManager::Ptr events);
    bool process(double dt,
                 component::BlobSolid& physic,
                 component::World& world);
private:
    typedef enum
    {
        Building,
        BackOff,
        WaitNobody
    } Action;
    Action state_;
};

}
}

#endif // BEBLOB_LOGIC_BUILD_BLOCK_H
