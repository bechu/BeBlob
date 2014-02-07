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
#include <sstream>

#include "components/land_render.h"
#include "components/blob_render.h"
#include "components/home.h"
#include "components/flower.h"
#include "components/teams_color.h"

#include "graphics.h"
#include "sf/curve.h"
#include "sf/particles/particle_system.h"
#include "graphic/texture_maker.h"
#include "components/window.h"

#include "graphic/widget.h"
#include "math/range.h"

namespace beblob
{
namespace system
{

void Graphics::configure()
{
    // wait event for launch particle
    subscribe<event::Particle::Smoke,
            event::Particle::Slap,
            event::Particle::Fireworks>();
    // create the world shape
    subscribe<event::World::MakeUp>();
    // to asserve blob eyes with the mouse ;)
    subscribe<event::Mouse::Move>();

    subscribe<event::Game::New>();
    subscribe<event::Game::End>();
}

void Graphics::initialize()
{
    window_ = createEntity();
    window_.assign<component::Window>(1000, 600);
    light_manager_.load(component::World::kWidth*component::LandSolid::kSize,
                        component::World::kHeight*component::LandSolid::kSize);

    sf::ZipStream zip = {"blob.pack"};
    zip.open("particle.png");
    particle_texture_.loadFromStream(zip);
    ps.setTexture(particle_texture_);
}

void Graphics::populate()
{
    auto Game = entities<component::World>()[0];
    auto world = *Game.component<component::World>();
    auto lands = entities<component::LandSolid>();
    auto Colors = Game.component<component::TeamsColor>();

    for(auto &land : lands)
    {
        sf::Vector2i c = component::World::screen2map(land.component<component::LandSolid>()->getCenter());
        // create land graphic entities
        if(world(c).type == component::World::Cell::kHome)
        {
            auto render = land.assign<component::Home>();

            int team = world.getTeamIndex(c);
            if(team == 0)
            {
                auto light = land.assign<component::Light>();
                light->setRadius(200);
                light->setColor(sf::Color(255, 255, 255, 255));
                light->setPosition(component::World::map2screen(c));
            }
            sf::Color color;
            if(team >= 0)
                color = Colors->Colors[team];
            render->compute(*land.component<component::LandSolid>(), color);
        }
        else
        {
            auto render = land.assign<component::LandRender>();
            render->compute(*land.component<component::LandSolid>());
        }
    }

    for(int i=0;i<component::World::kWidth;++i)
    {
        for(int j=0;j<component::World::kHeight;++j)
        {
            sf::Vector2i c(i, j);
            // create all flowers
            if(world(c).type == component::World::Cell::kFood)
            {
                auto e = createEntity();
                auto flower = e.assign<component::Flower>();
                flower->setPosition( component::World::map2screen(c) );
            }
        }
    }

    window_.component<component::Window>()->setCenter(component::World::map2screen(world.getHome(0)));
    emit<event::Game::Start>();
}

void Graphics::render_world(double dt)
{
    if(entities<component::World>().size() == 0) return ;
    auto Game = entities<component::World>()[0];
    auto window = window_.component<component::Window>();
    auto world = *Game.component<component::World>();
    sf::RenderWindow& render = *((*window)().get());

    render.clear(sf::Color::Black);

    light_manager_.clear();

    for(auto land : entities<component::LandRender>())
    {
        component::LandRender::Ptr l = land.component<component::LandRender>();
        render.draw(*l);
    }

    for(auto blob : entities<component::BlobRender>())
    {
        auto P = blob.component<component::BlobSolid>();
        auto G = blob.component<component::BlobRender>();
        auto L = blob.component<component::BlobAI>();
        G->compute(dt, *P, *L, mouse_);
        render.draw(*G);
        if(L->getTeamId() == 0)
        {
            auto light = blob.component<component::Light>();
            light->setPosition(P->getPosition());
            light->setRadius(300);
            light_manager_.addLight(light);
        }
    }

    for(auto flower : entities<component::Flower>())
    {
        auto g = flower.component<component::Flower>();
        sf::Vector2i c = component::World::screen2map(g->getPosition());
        g->compute(world(c).Food);
        render.draw(*g);
    }

    for(auto home : entities<component::Home>())
    {
        auto light = home.component<component::Light>();
        if(light)
        {
            light_manager_.addLight(light);
        }
        render.draw(*home.component<component::Home>());
    }

    if(end_ == false)
    {
        light_manager_.compute();
        light_manager_.draw(render);
    }
}

void Graphics::update(double dt)
{
    auto window = window_.component<component::Window>();

    window->updateView();

    ps.update(dt);

    sf::RenderWindow& render = *((*window)().get());

    render.clear(sf::Color(20, 20, 20));

    render_world(dt);

    render.draw(ps);

    window->uiView();

    for(auto entity : entities<component::Widget>())
    {
        int count = entity.count<component::Widget>();
        for(int i=0;i<count;++i)
        {
            render.draw(*entity.component<component::Widget>(i));
        }
    }

    render.display();
}


void Graphics::receive(const event::Particle::Smoke &event)
{
    ps.addEmitter(sf::Firework(event.Position), sf::seconds(.5));
}

void Graphics::receive(const event::Particle::Slap& event)
{
    ps.addEmitter(sf::Firework(event.Position), sf::seconds(.1));
}

void Graphics::receive(const event::Particle::Fireworks& event)
{
    ps.addEmitter(sf::Firework(event.Position, event.Color), sf::seconds(.3));
}

void Graphics::receive(const event::World::MakeUp& event)
{
    static int u = 0;
    u++;

    // pre-setup fo all flower states
    component::Flower::prepare(event.Graine);

    // pre-render now
    for(auto state : math::Range(0, component::Flower::kMaxState))
    {
        component::Flower::Draw draw;
        component::Flower::createTexture(state, event.Graine, draw);
        std::shared_ptr<sf::RenderTexture> texture = component::Flower::drawTexture(draw);
        texture->display();
    }

    // create the wall texture for the world shape
    procedural::TextureMaker make(event.Graine, component::World::kWidth*component::LandSolid::kSize,
                                  component::World::kHeight*component::LandSolid::kSize);

    math::Random rng;
    rng.setSeed(event.Graine);

    switch(rng.next(3))
    {
    case 0:
        make(0).pattern<procedural::RadialPatternGenerator>(8+rng.next(100),
                                                            sf::Vector2f(rng.nextFloat(), rng.nextFloat()));
        break;
    case 1:
        make(0).pattern<procedural::DiagonalPatternGenerator>(
                    8+rng.next(100),
                    5+rng.next(200));
        break;
    case 2:
        make(0).pattern<procedural::VerticalPatternGenerator>(
                    5+rng.next(20));
        break;
    case 3:
        make(0).pattern<procedural::HorizontalPatternGenerator>(
                    5+rng.next(20));
        break;
    }

    //make(0).chartColors<procedural::GoldenRatioGradientColorGenerator>(event.Graine);

    //make(1).pattern<procedural::NoisePatternGenerator>();

    //component::LandRender::texture.loadFromImage(make.pop());
    populate();
}

void Graphics::receive(const event::Mouse::Move& event)
{
    mouse_ = event.Position;
}

void Graphics::receive(const event::Game::New& UNUSED(event))
{
    end_ = false;
    component::Flower::clear();
    for(auto &e : entities<component::LandRender>()) e.destroy();
    for(auto &e : entities<component::Home>()) e.destroy();
    for(auto &e : entities<component::Flower>()) e.destroy();
    for(auto &e : entities<component::BlobRender>()) e.destroy();
}

void Graphics::receive(const event::Game::End& event)
{
    end_ = true;
}

}
}
