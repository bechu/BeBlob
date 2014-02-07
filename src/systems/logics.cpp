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

#include <iostream>

#include "logics.h"

#include "components/blob_ai.h"
#include "components/blob_render.h"
#include "components/blob_solid.h"
#include "components/land_render.h"
#include "components/home.h"
#include "components/land_physic.h"
#include "components/expert.h"
#include "components/flower.h"
#include "components/teams_color.h"
#include "components/light.h"

#include "graphic/chart_colors.h"
#include "ai/astar.h"
#include "physic/surface.h"
#include "ai/job.h"
#include "ai/jobs/break_block.h"
#include "ai/jobs/goto.h"
#include "ai/jobs/eat_food.h"
#include "ai/jobs/build_block.h"
#include "math/range.h"

namespace beblob
{
namespace system
{
using namespace component;

void Logics::configure()
{
    subscribe<event::Blob::New,
            event::Blob::Collide,
            event::Blob::Remove,
            event::Blob::Split,
            event::Blob::BuildBlock>();
    subscribe<event::Blob::Job>();

    subscribe<event::World::AddBlock>();

    subscribe<event::Game::Build,
            event::Game::Start>();
    subscribe<event::Game::New>();

    subscribe<event::UI::Pause,
            event::UI::Resume>();
}

void Logics::initialize()
{
}

void Logics::release()
{
}

void Logics::update(double dt)
{
    if(disable_) return;
    if(Game.valid() == false) return ;
    bool gameLoop = false;
    auto world = Game.component<component::World>();
    timeout_ -= dt;
    if(timeout_ < 0)
    {
        gameLoop = true;
        timeout_ = kGameLoopTime;
    }

    for(auto& blob : entities<component::BlobAI>())
    {
        auto L = blob.component<component::BlobAI>();
        auto P = blob.component<component::BlobSolid>();
        L->update(dt, *P, *world);

        if(gameLoop)
        {
            if(L->gameLoop(blob, *world, event_manager_) == utils::Job::kEating)
                if(L->getTeamId() == 0)
                    emit<event::Sound::Play>("eat.ogg");
        }

        if(L->state() >= 1.0f)
            emit<event::Blob::Split>(blob);
        else if(L->state() <= 0.f)
            emit<event::Blob::Remove>(blob);
    }

    if(gameLoop)
    {
        for(auto expert : entities<component::Expert>())
        {
            expert.component<component::Expert>()->notify(*expert.component<component::Team>());
        }
    }
}

void Logics::receive(const event::Blob::Remove& event)
{
    ecs::Entity blob = event.Blob;
    component::BlobAI::Ptr L = blob.component<component::BlobAI>();
    Teams[L->getTeamId()].component<component::Team>()->removeMember(blob);
    blob.destroy();
    //if(Teams[L->getTeamId()].component<component::Team>()->size() == 0)
    //{
    //    emit<event::Game::End>(L->getTeamId() == 0);
    //}
}

void Logics::receive(const event::Blob::New& event)
{
    ecs::Entity blob = createEntity();
    blob.assign<component::BlobSolid>(event.Position);
    blob.assign<component::BlobAI>(event.Team);
    auto light = blob.assign<component::Light>();
    light->setRadius(200);
    light->setColor(sf::Color(255, 255, 255, 255));

    auto G = blob.assign<component::BlobRender>();

    G->setFillColor(event.Color);

    Teams[event.Team].component<component::Team>()->addMember(blob);
}

void Logics::receive(const event::Blob::Split& event)
{
    sf::Vector2f pos = event.Blob.component<component::BlobSolid>()->getPosition();
    auto L = event.Blob.component<component::BlobAI>();
    auto G = event.Blob.component<component::BlobRender>();
    if(L->getTeamId() == 0)
        emit<event::Sound::Play>("split.ogg");
    emit<event::Blob::Remove>(event.Blob);
    emit<event::Blob::New>(sf::Vector2f(pos.x-component::BlobSolid::kDefaultRadius/2, pos.y), L->getTeamId(),
                           G->getFillColor());
    emit<event::Blob::New>(sf::Vector2f(pos.x+component::BlobSolid::kDefaultRadius/2, pos.y),
                           L->getTeamId(),
                           G->getFillColor());
}

void Logics::receive(const event::Blob::Collide& event)
{
    auto world = Game.component<component::World>();
    auto L = event.Blob.component<component::BlobAI>();
    auto P = event.Blob.component<component::BlobSolid>();
    if(math::distance<float>(P->getPosition(),
                             component::World::map2screen(L->getGoal())) < component::LandSolid::kSize)
    {
        if(L->getTeamId() == 0)
            emit<event::Sound::Play>("break.ogg");
        if(L->collide(*world))
        {
            emit<event::Particle::Slap>(component::World::map2screen(L->getGoal()));
            sf::Vector2i pos = L->getGoal();
            (*world).setType(pos, World::Cell::kNothing);
            emit<event::Sound::Play>("fireworks.ogg");
            at<event::World::DeleteBlock>(std::chrono::milliseconds(400), pos);
            if(L->getGoal() == world->getHome(0))
            {
                emit<event::Game::End>(false);
            }
            else if(L->getGoal() == world->getHome(1))
            {
                emit<event::Game::End>(true);
            }
        }
    }
}

void Logics::giveJob(utils::Job::Ptr job)
{
    for(auto blob : entities<component::BlobAI>())
    {
        auto L = blob.component<component::BlobAI>();
        if(L->select())
        {
            L->giveJob(job);
            // Node<utils::Job, component::BlobAI>().run();
            L->select() = false;
        }
    }
}

template <typename T>
void Logics::giveJob(const sf::Vector2i& topleft, const sf::Vector2i& bottomright, component::World::Cell::Type type)
{
    auto world = *Game.component<component::World>();
    for(auto x = topleft.x;x < bottomright.x;++x)
    {
        for(auto y = topleft.y;y < bottomright.y;++y)
        {
            sf::Vector2i c(x, y);
            if(world(c).type == type)
            {
                giveJob(std::make_shared<T>(c));
            }
        }
    }
}

void Logics::receive(const event::Blob::Job& event)
{
    auto world = *Game.component<component::World>();

    if(event.Left == false)
    {
        switch(world(event.Pos).type)
        {
        case component::World::Cell::kWall:
        case component::World::Cell::kHome:
            event.Blob.component<component::BlobAI>()->giveJob(std::make_shared<utils::BreakBlock>(event.Pos));
            break;
        case component::World::Cell::kNothing:
            event.Blob.component<component::BlobAI>()->giveJob(std::make_shared<utils::BuildBlock>(event.Pos));
            break;
        case component::World::Cell::kFood:
            event.Blob.component<component::BlobAI>()->giveJob(std::make_shared<utils::EatFood>(event.Pos));
            break;
        default:
            break;
        }
    }
    else
    {
        switch(world(event.Pos).type)
        {
        case component::World::Cell::kWall:
        case component::World::Cell::kHome:
            event.Blob.component<component::BlobAI>()->giveJob(std::make_shared<utils::BreakBlock>(event.Pos));
            break;
        case component::World::Cell::kNothing:
            event.Blob.component<component::BlobAI>()->giveJob(std::make_shared<utils::GoTo>(event.Pos));
            break;
        case component::World::Cell::kFood:
            break;
        default:
            break;
        }
    }
}

void Logics::receive(const event::World::AddBlock& event)
{
    auto world = Game.component<component::World>();
    world->setType(event.Position, component::World::Cell::kWall);
    world->setResistance(event.Position, component::World::Cell::kDefaultResistance);
}

void Logics::receive(const event::Blob::BuildBlock& event)
{
    giveJob(std::make_shared<utils::BuildBlock>(event.Position));
}

void Logics::receive(const event::Game::Build& event)
{
    Game = createEntity();
    component::Team::reset();
    component::World::Ptr world = Game.assign<component::World>();
    component::TeamsColor::Ptr Colors = Game.assign<component::TeamsColor>();
    world->create(event.Graine, event.TeamCount);
    team_count_ = event.TeamCount;
    int cpt = 0;
    procedural::ChartColors<procedural::GoldenRatioGradientColorGenerator> p(event.Graine);
    while(cpt < event.TeamCount)
    {
        ecs::Entity e = createEntity();
        if(cpt > 0)
        {
            Colors->Colors.push_back(p.next());
            auto bot = e.assign<component::Expert>(*world);
            world->addObserver(bot);
        }
        else
        {
            Colors->Colors.push_back(event.Color);
        }
        e.assign<component::Team>();
        Teams.push_back(e);
        cpt++;
    }
    emit<event::World::Shape>(Game, event.Graine);
}

void Logics::receive(const event::Game::New& UNUSED(event))
{
    for(auto &e : entities<component::Expert>())
    {
        e.component<component::Expert>()->exit();
        e.destroy();
    }
    auto world = Game.component<component::World>();
    world.reset();
    for(auto &e : entities<component::World>()) e.destroy();
    Teams.clear();
}

void Logics::receive(const event::Game::Start& event)
{
    disable_ = false;
    auto world = Game.component<component::World>();
    assert(world);
    TeamsColor::Ptr colors = Game.component<component::TeamsColor>();
    assert(colors);

    for(auto i : math::Range(0, team_count_))
    {
        sf::Vector2f pos =
                World::map2screen(world->getHome(i)-sf::Vector2i(0, 1));
        emit<event::Blob::New>(pos, i, colors->Colors[i]);
    }
}

void Logics::receive(const event::UI::Pause& event)
{
    disable_ = true;
}

void Logics::receive(const event::UI::Resume& event)
{
    disable_ = false;
}

}
}
