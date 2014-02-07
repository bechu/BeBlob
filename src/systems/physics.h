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

#ifndef BEBLOB_PHYSICS_H
#define BEBLOB_PHYSICS_H

#include <iostream>
#include <memory>

#include "ecs/ecs.h"
#include "ai/job.h"
#include "common/unused.h"
#include "common/events.h"

#include "physic/quadtree.h"
#include "components/land_physic.h"

#include "components/env.h"

namespace beblob
{
namespace system
{

class Physics
        : public ecs::System<Physics>
{
public:
    Physics();
    virtual ~Physics();
    void configure();
    void initialize();
    void update(double dt);
    void release() {}

    void receive(const event::OnKey& event);
    void receive(const event::Mouse::Click& event);
    void receive(const event::World::DeleteBlock& event);
    void receive(const event::World::AddBlock& event);
    void receive(const event::World::Shape& event);
    void receive(const event::Mouse::Move& event);
    void receive(const event::UI::Pause& event);
    void receive(const event::UI::Resume& event);
    void receive(const event::Game::New& event);

    ecs::Entity getLand(component::LandSolid::Ptr land);
private:
    ecs::Entity Game;
    bool disable = false;
    physic::QuadTree quadtree_;
    component::Env env_;
    ecs::Entity selected_;
};

}
}

#endif // BEBLOB_PHYSICS_H
