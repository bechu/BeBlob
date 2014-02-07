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

#include <map>
#include <algorithm>
#include <random>
#include <memory>

#include "expert.h"
#include "blob_ai.h"
#include "ai/jobs/eat_food.h"
#include "ai/jobs/break_block.h"
#include "ai/jobs/goto.h"
#include "ai/jobs/build_block.h"
#include "blob_solid.h"
#include "math/vector.h"

namespace beblob
{
namespace component
{

Expert::Expert(const World &world) :
    world_mutex_(),
    mutex_(),
    cond_(),
    thread_(&Expert::run, this)
{
    world_ = world;
    thread_.launch();
}

Expert::~Expert()
{
    exit();
}

void Expert::updated(const sf::Vector2i& c, const component::World::Cell& cell)
{
    std::lock_guard<std::mutex> lock(world_mutex_);
    world_.set(c, cell);
}

void Expert::run()
{
    while(run4ever_)
    {
        { // waiting wake up
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock);
            if(!run4ever_) break;
        }

        // ok i'll touch my own data ;)
        std::lock_guard<std::mutex> lock(world_mutex_);

        // the ai consist of :
        // - eat enough to increase team size
        strenghnessTeam_.proceed(world_, team_);
        // - build block around home to protect it
        consolidateHome_.proceed(world_, team_);
        // - attack the other team
        attack_.proceed(world_, team_);
    }
}

void Expert::notify(component::Team& team)
{
    std::unique_lock<std::mutex> lock(mutex_);
    for(auto& m : team_)
    {
        if(m.job != nullptr)
        {
            for(uint32_t i=0;i<team.size();++i)
            {
                ecs::Entity blob = team.getMember(i);
                if(blob.id() == m.id)
                {
                    auto L = blob.component<component::BlobAI>();
                    L->giveJob(m.job);
                    break;
                }
            }
        }
    }
    // ok create a local copy of the team for my ai
    team_.clear();
    for(uint32_t i=0;i<team.size();++i)
    {
        team_.id = team.getID();
        utils::Member m;
        ecs::Entity blob = team.getMember(i);
        if(blob.valid())
        {
            auto P = blob.component<component::BlobSolid>();
            assert(P);
            m.position = component::World::screen2map(P->getPosition());
            auto L = blob.component<component::BlobAI>();
            assert(L);
            m.goal = L->getGoal();
            m.id = blob.id();
            m.busy = L->isBusy();
            m.state = L->state();
            team_.push_back(m);
        }
    }
    team_.sort_hungry();
    cond_.notify_one();
}

void Expert::exit()
{
    team_.clear();
    run4ever_ = false;
    cond_.notify_one();
    thread_.wait();
}

}
}
