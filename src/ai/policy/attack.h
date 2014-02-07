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

#ifndef BEBLOB_AI_ATTACK_H
#define BEBLOB_AI_ATTACK_H

#include "ai/policy.h"
#include "components/team.h"

namespace beblob
{
namespace utils
{

class Attack
        : public Policy
{
public:
    void proceed(component::World& world, utils::Team& team);
private:
    void select_team(utils::Team& team);
    bool team_selected_ = false;
    component::Team::Id other_team_id_;
};

}
}

#endif // BEBLOB_AI_ATTACK_H
