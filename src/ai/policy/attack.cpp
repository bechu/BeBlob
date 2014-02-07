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

#include "attack.h"
#include "components/blob_ai.h"
#include "ai/jobs/break_block.h"

namespace beblob
{
namespace utils
{

void Attack::proceed(component::World& world, utils::Team& team)
{
    if(!team_selected_) select_team(team);

    computeGoals(team);
    for(auto& blob : team)
    {
        if(blob.busy == true) continue;

        sf::Vector2i loc = findTask(world,
        {component::World::Cell::kHome},
                                    world.getHome(other_team_id_),
                                    blob.position,
                                    1,
                                    false);
        if(loc.x != -1)
        {
            blob.job = std::make_shared<utils::BreakBlock>(world.getHome(other_team_id_));
            goals_.push_back(world.getHome(other_team_id_));
            blob.busy = true;
            continue;
        }

        loc = findTask(world,
        {component::World::Cell::kWall},
                                    world.getHome(other_team_id_),
                                    blob.position,
                                    30,
                                    false);
        if(loc.x != -1)
        {
            goals_.push_back(loc);
            blob.job = std::make_shared<utils::BreakBlock>(loc);
            blob.busy = true;
            continue;
        }
    }
}

void Attack::select_team(utils::Team& team)
{
    component::Team::Id count = component::Team::countTeam();
    math::Random rng;
    if(count == 1) return ;
    do
    {
        other_team_id_ = rng.rangeInt(0, count-1);
    } while(other_team_id_ == team.id);
    team_selected_ = true;
}

}
}
