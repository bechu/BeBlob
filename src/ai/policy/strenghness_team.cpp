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

#include "strenghness_team.h"
#include "components/blob_ai.h"
#include "ai/jobs/eat_food.h"

namespace beblob
{
namespace utils
{

void StrenghnessTeam::proceed(component::World& world, utils::Team& team)
{
    computeGoals(team);
    for(auto &blob : team)
    {
        if(blob.busy == true) continue;
        if(blob.state < 0.25 || team.size() < 5)
        {
            sf::Vector2i loc = findTask(world,
            {component::World::Cell::kFood},
                                        blob.position,
                                        blob.position,
                                        kMaxDistanceForEat);
            if(loc.x != -1)
            {
                goals_.push_back(loc);
                blob.job = std::make_shared<utils::EatFood>(loc);
                blob.busy = true;
            }
        }
    }
}

}
}
