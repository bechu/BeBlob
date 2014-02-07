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

#include "consolidate_home.h"
#include "components/blob_ai.h"
#include "ai/jobs/build_block.h"

namespace beblob
{
namespace utils
{

void ConsolidateHome::proceed(component::World& world, utils::Team& team)
{
    computeGoals(team);
    if(computeDensity(world, team) < kDensityAim)
    {
        for(auto &blob : team)
        {
            if(blob.busy == true) continue;
            sf::Vector2i loc = findTask(world,
            {component::World::Cell::kNothing, component::World::Cell::kFood},
                                        world.getHome(team.id),
                                        blob.position,
                                        30);
            if(loc.x != -1)
            {
                goals_.push_back(loc);
                blob.job = std::make_shared<utils::BuildBlock>(loc);
                blob.busy = true;
            }
        }
    }
}

float ConsolidateHome::computeDensity(const component::World& world, utils::Team& team) const
{
    double count_wall = 0;
    int offset = kRadiusConsolidation;

    sf::Vector2i position = world.getHome(team.id);

    for(int i=position.x-offset;i<position.x+offset;++i)
    {
        for(int j=position.y-offset;j<position.y+offset;++j)
        {
            if(world(sf::Vector2i(i, j)).isCollisionable())
            {
                count_wall++;
            }
        }
    }
    return count_wall / double((offset*2)*(offset*2));
}

}
}
