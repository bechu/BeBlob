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

#ifndef BEBLOB_BOT_H
#define BEBLOB_BOT_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <SFML/System/Thread.hpp>

#include "ecs/ecs.h"
#include "world.h"
#include "ai/job.h"
#include "ai/policy.h"
#include "ai/policy/strenghness_team.h"
#include "ai/policy/consolidate_home.h"
#include "ai/policy/attack.h"
#include "components/team.h"

namespace beblob
{
namespace component
{

////////////////////////////////////////////////////////////
/// \brief the bot definition
////////////////////////////////////////////////////////////
class Expert
        : public ecs::Component<Expert>
        , public component::World::Observer
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief the bot constructor
    ////////////////////////////////////////////////////////////
    Expert(const World& world);

    ////////////////////////////////////////////////////////////
    /// \brief destructor
    ////////////////////////////////////////////////////////////
    virtual ~Expert();

    ////////////////////////////////////////////////////////////
    /// \brief method call when the world was updated
    ///
    /// \param c cell coordinate
    /// \param cell new cell values
    ////////////////////////////////////////////////////////////
    void updated(const sf::Vector2i& c, const component::World::Cell& cell);

    ////////////////////////////////////////////////////////////
    /// \brief stop the thread and exit
    ////////////////////////////////////////////////////////////
    void exit();

    ////////////////////////////////////////////////////////////
    /// \brief something append wake up the thread
    ////////////////////////////////////////////////////////////
    void notify(component::Team &team);
private:
    ////////////////////////////////////////////////////////////
    /// \brief thinking about the strategy
    ////////////////////////////////////////////////////////////
    void run();

    /// the world protection
    std::mutex world_mutex_;
    /// local copy of world
    World world_;
    //// mutex for wake up
    std::mutex mutex_;
    /// condidition wake up
    std::condition_variable cond_;
    /// the thread
    sf::Thread thread_;
    /// until true run the thread
    bool run4ever_ = true;
    /// consolidate home policy
    utils::ConsolidateHome consolidateHome_;
    /// increase team members policy
    utils::StrenghnessTeam strenghnessTeam_;
    /// attach a team
    utils::Attack attack_;
    /// local team storage
    utils::Team team_;
};

}
}

#endif // BEBLOB_BOT_H
