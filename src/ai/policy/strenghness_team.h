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

#ifndef BEBLOB_AI_POLICY_STRENG_TEAM_H
#define BEBLOB_AI_POLICY_STRENG_TEAM_H

#include "ai/policy.h"

namespace beblob
{
namespace utils
{

class StrenghnessTeam
        : public Policy
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief apply the strenghness strategy
    ///
    /// \param world the game's world
    /// \param team team of the blob
    ////////////////////////////////////////////////////////////
    void proceed(component::World& world, utils::Team& team);
private:
    /// arbritrary distance max value to get food
    const static int kMaxDistanceForEat = 30;
};

}
}

#endif // BEBLOB_AI_POLICY_STRENG_TEAM_H
