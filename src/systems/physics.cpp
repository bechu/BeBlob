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

#include "physics.h"

#include "components/blob_render.h"
#include "components/blob_solid.h"
#include "components/land_physic.h"
#include "physic/surface.h"
#include "components/blob_ai.h"
#include "components/land_render.h"
#include "components/land_physic.h"
#include "components/world.h"
#include "ai/jobs/goto.h"

namespace beblob
{

namespace system
{

Physics::Physics()
    : quadtree_(0, 0,
                component::World::kWidth*component::LandSolid::kSize,
                component::World::kHeight*component::LandSolid::kSize)
{
}

Physics::~Physics()
{
}

void Physics::configure()
{
    subscribe<event::Mouse::Click,
            event::Mouse::Move>();
    subscribe<event::OnKey>();
    subscribe<event::World::DeleteBlock,
            event::World::AddBlock>();
    subscribe<event::UI::Pause,
            event::UI::Resume>();
    subscribe<event::World::Shape>();
    subscribe<event::Game::New>();
}

void Physics::initialize()
{
}

void Physics::update(double dt)
{
    if(disable) return ;
    auto world = Game.component<component::World>();

    for(auto &entity : entities<component::BlobSolid>())
    {
        auto P = entity.component<component::BlobSolid>();

        P->move(0.01*dt);
        P->satisfy();

        std::set<component::LandSolid::Ptr> lands;
        std::map<component::LandSolid::Ptr, sf::Vector2f> where;

        for(unsigned int i = 0; i < component::BlobSolid::kNumPointMasses; i++)
        {
            physic::PointMass& p = P->getPointMasse(i);
            if(world->operator()(component::World::screen2map(p.currPos) ).isCollisionable() == false)

            {
                continue;
            }
            physic::QuadTree::Object::List list = quadtree_.get(p.currPos.x,
                                                                p.currPos.y);
            for(auto &object : list)
            {
                auto land = std::static_pointer_cast<component::LandSolid>(object);
                if(P->collisionTest(*land, i))
                {
                    lands.insert(land);
                    where[land] = P->getPointMasse(i).currPos;
                }
            }
        }

        P->collide(lands);
        for(auto land : lands)
        {
            if(P->haveCollided(land))
            {
                emit<event::Blob::Collide>(entity,
                                           getLand(land),
                                           where[land]);
            }
        }

        // test collision beetwen each other blobs
        for(auto e : entities<component::BlobSolid>())
        {
            auto p = e.component<component::BlobSolid>();
            if(p)
            {
                if(P == p) continue ;
                P->collisionTest(*p);
            }
        }

        P->collisionTest(env_);
    }
}

void Physics::receive(const event::OnKey& event)
{
    if(event.Key == sf::Keyboard::P)
    {
        disable = !disable;
    }
}

void Physics::receive(const event::Mouse::Click& event)
{
    if(disable) return ;
    for(auto entity : entities<component::BlobSolid>())
    {
        auto P = entity.component<component::BlobSolid>();
        auto L = entity.component<component::BlobAI>();
        if(L->getTeamId() == 0)
        {
            if(P->contains(event.Position))
            {
                L->select() = true;
                L->cancel(*P);
                selected_ = entity;
                return ;
            }
            else
                L->select() = false;
        }
    }
    if(selected_.valid())
    {
        emit<event::Blob::Job>(selected_,
                               component::World::screen2map(event.Position),
                               event.Button == sf::Mouse::Left);
    }
    selected_.invalidate();
}

ecs::Entity Physics::getLand(component::LandSolid::Ptr land)
{
    for(auto e : entities<component::LandSolid>())
    {
        if(land.get() == e.component<component::LandSolid>().get())
        {
            return e;
        }
    }
    return ecs::Entity();
}

void Physics::receive(const event::World::DeleteBlock& event)
{
    sf::Vector2f real = component::World::map2screen(event.Position);
    physic::QuadTree::Object::List list = quadtree_.get(real.x, real.y);
    for(auto &object : list)
    {
        auto land = std::static_pointer_cast<component::LandSolid>(object);
        if(land->contains(event.Position))
        {
            physic::Surface s;
            s.Min = land->Min;
            s.Max = land->Max;
            quadtree_.remove(object);
            ecs::Entity e = getLand(land);
            e.destroy();
            std::vector<physic::Surface> ss = s.remove(event.Position);
            for(auto surface : ss)
            {
                ecs::Entity land = createEntity();
                auto physic = land.assign<component::LandSolid>();
                auto render = land.assign<component::LandRender>();
                surface.publish(*physic);
                physic->Min = surface.Min;
                physic->Max = surface.Max;
                render->compute(*physic);
                quadtree_.add(physic);
            }
            break;
        }
    }
}

void Physics::receive(const event::World::AddBlock& event)
{
    physic::Surface surface(event.Position);
    ecs::Entity land = createEntity();
    auto physic = land.assign<component::LandSolid>();
    auto render = land.assign<component::LandRender>();
    surface.publish(*physic);
    physic->Min = surface.Min;
    physic->Max = surface.Max;
    render->compute(*physic);
    quadtree_.add(physic);
}

void Physics::receive(const event::World::Shape& event)
{
    Game = event.Game;
    auto world = event.Game.component<component::World>();
    physic::Surfaces ss;
    physic::Surfaces homes;

    for(int i=0;i<component::World::kWidth;++i)
    {
        for(int j=0;j<component::World::kHeight;++j)
        {
            sf::Vector2i c(i, j);
            if(world->operator()(c).type == component::World::Cell::kWall)
            {
                ss.append( physic::Surface(c) );
            }
            else if(world->operator()(c).type == component::World::Cell::kHome)
            {
                homes.append( physic::Surface(c) );
            }
        }
    }

    for(auto surface : ss)
    {
        ecs::Entity land = createEntity();
        auto physic = land.assign<component::LandSolid>();

        surface.publish(*physic);
        physic->Min = surface.Min;
        physic->Max = surface.Max;
        quadtree_.add(physic);
    }
    for(auto surface : homes)
    {
        ecs::Entity land = createEntity();
        auto physic = land.assign<component::LandSolid>();

        surface.publish_home(*physic);
        physic->Min = surface.Min;
        physic->Max = surface.Max;
        quadtree_.add(physic);
    }

    emit<event::World::MakeUp>(event.Game, event.Graine);
}

void Physics::receive(const event::Mouse::Move& event)
{
    for(auto entity : entities<component::BlobSolid>())
    {
        auto P = entity.component<component::BlobSolid>();
        auto L = entity.component<component::BlobAI>();
        L->focus() = P->contains(event.Position);
    }
    if(selected_.valid())
    {
        selected_.component<component::BlobAI>()->
                giveJob(std::make_shared<utils::GoTo>(component::World::screen2map(event.Position)));
    }
}

void Physics::receive(const event::UI::Pause& event)
{
    disable = true;
    selected_.invalidate();
}

void Physics::receive(const event::UI::Resume& event)
{
    disable = false;
}

void Physics::receive(const event::Game::New& event)
{
    disable = false;
}

}
}
