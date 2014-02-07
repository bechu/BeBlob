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

#ifndef BEBLOB_GRAPHICS_H
#define BEBLOB_GRAPHICS_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "ecs/ecs.h"
#include "common/unused.h"
#include "common/events.h"
#include "sf/particles/particle_system.h"
#include "graphic/light_manager.h"
#include "sf/zip_stream.h"

namespace beblob
{
namespace system
{

class Graphics
        : public ecs::System<Graphics>
{
public:
    void configure();
    void initialize();
    void update(double dt);
    void release() {}

    void receive(const event::World::MakeUp &event);
    void receive(const event::Particle::Smoke& event);
    void receive(const event::Particle::Slap& event);
    void receive(const event::Mouse::Move& event);
    void receive(const event::Particle::Fireworks& event);
    void receive(const event::Game::New& event);
    void receive(const event::Game::End& event);
private:
    void render_world(double dt);
    void populate();
    /// touch the window
    ecs::Entity window_;
    /// keep the last mouse move value to help blob to follow mouse
    sf::Vector2f mouse_;
    /// light manager
    LightManager light_manager_;
    /// to have fireworks
    sf::ParticleSystem ps;
    /// it's the end of the game ?
    bool end_ = true;
    /// particle texture
    sf::Texture particle_texture_;
};

}
}

#endif // BEBLOB_GRAPHICS_H
