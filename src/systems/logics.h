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

#ifndef BEBLOB_LOGICS_H
#define BEBLOB_LOGICS_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>

#include "math/random.h"
#include "ecs/ecs.h"
#include "common/events.h"
#include "components/world.h"
#include "ai/job.h"
#include "graphic/chart_colors.h"

namespace beblob
{
namespace system
{

class Logics
        : public ecs::System<Logics>
{
public:
    void configure();
    void initialize();
    void update(double dt);
    void release();

    void receive(const event::Blob::Collide& event);
    void receive(const event::Blob::New& event);
    void receive(const event::Blob::Remove& event);
    void receive(const event::Blob::Split& event);
    void receive(const event::Blob::BuildBlock& event);
    void receive(const event::Blob::Job& event);

    void receive(const event::World::AddBlock& event);

    void receive(const event::Game::Build& event);
    void receive(const event::Game::Start& event);
    void receive(const event::Game::New& event);

    void receive(const event::UI::Pause& event);
    void receive(const event::UI::Resume& event);

private:
    /// every game loop time (in ms) the ai compute ideas
    constexpr static int kGameLoopTime = 1000;

    template <typename T>
    void giveJob(const sf::Vector2i& topleft,
                 const sf::Vector2i& bottomright,
                 component::World::Cell::Type type);

    void giveJob(utils::Job::Ptr job);

    /// default value for timeout
    double timeout_ = kGameLoopTime;
    /// a copy of the game entity
    ecs::Entity Game;
    /// number of team
    int team_count_ = 0;
    /// list of team
    std::vector<ecs::Entity> Teams;
    /// disable/enable the logic system
    bool disable_ = false;
};

}
}

#endif // BEBLOB_LOGICS_H
