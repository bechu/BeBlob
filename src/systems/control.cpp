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

#include "control.h"
#include "common/events.h"
#include "components/button.h"
#include "components/seed_input.h"
#include "components/color_picker.h"
#include "components/image.h"
#include "components/label.h"

namespace beblob
{
namespace system
{

void Control::configure()
{
    subscribe<event::UI::Button,
            event::UI::ColorPicker,
            event::UI::SelectSeed,
            event::UI::Pause,
            event::UI::Resume>();
    subscribe<event::Game::Start>();
    subscribe<event::Game::New>();
    subscribe<event::Game::End>();
}

void Control::initialize()
{
    auto es = entities<component::Window>();
    window_ = es[0].component<component::Window>();
    gui_ = createEntity();
    emit<event::Game::New>();
}

namespace
{

sf::IntRect getViewport(const sf::View& view)
{
    float width  = static_cast<float>(view.getSize().x);
    float height = static_cast<float>(view.getSize().y);
    const sf::FloatRect& viewport = view.getViewport();

    return sf::IntRect(static_cast<int>(0.5f + width  * viewport.left),
                       static_cast<int>(0.5f + height * viewport.top),
                       static_cast<int>(width  * viewport.width),
                       static_cast<int>(height * viewport.height));
}

sf::Vector2f mapPixelToCoords(const sf::Vector2i& point, const sf::View& view)
{
    sf::Vector2f normalized;
    sf::IntRect viewport = getViewport(view);
    normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.width;
    normalized.y =  1.f - 2.f * (point.y - viewport.top)  / viewport.height;
    return view.getInverseTransform().transformPoint(normalized);
}

}

void Control::update(double dt)
{
    if(!window_) return ;
    if(!((*window_)())->isOpen()) return ;
    sf::RenderWindow& render = *((*window_)().get());
    sf::Event event;

    while(render.pollEvent(event))
    {
        if(!run_)
        {
            emit<ecs::Manager::Terminate>();
            return ;
        }
        if (event.type == sf::Event::Closed)
        {
            emit<ecs::Manager::Terminate>();
            return ;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                if(playing_ == true) emit<event::UI::Pause>();
                return ;
            }
        }

        if (event.type == sf::Event::KeyPressed)
        {
            for(auto entity : entities<component::Widget>())
            {
                int count = entity.count<component::Widget>();
                for(int i=0;i<count;++i)
                {
                    auto widget = entity.component<component::Widget>(i);
                    widget->onKey(event.key.code);
                }
            }
        }
        else if(event.type == sf::Event::TextEntered)
        {
            for(auto entity : entities<component::Widget>())
            {
                int count = entity.count<component::Widget>();
                for(int i=0;i<count;++i)
                {
                    auto widget = entity.component<component::Widget>(i);
                    widget->onUnicode(event.text.unicode);
                }
            }
        }

        // mouse handle
        sf::Vector2i mouse;
        if(event.type == sf::Event::MouseMoved)
            mouse = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
        else
            mouse = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f ui_position = mapPixelToCoords(mouse, render.getDefaultView());
        render.setView(window_->fixed);
        sf::Vector2f map_position = render.mapPixelToCoords(mouse);
        if(event.type == sf::Event::MouseMoved)
        {
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            for(auto entity : entities<component::Widget>())
            {
                int count = entity.count<component::Widget>();
                for(int i=0;i<count;++i)
                {
                    auto widget = entity.component<component::Widget>(i);
                    if(widget->contains(ui_position))
                    {
                        widget->onClick(ui_position, map_position);
                        return ;
                    }
                    else
                        widget->onMouseOut();
                }
            }
        }

        treatEventForGame(event, ui_position, map_position);
    }
    for(auto entity : entities<component::Widget>())
    {
        int count = entity.count<component::Widget>();
        for(int i=0;i<count;++i)
        {
            auto widget = entity.component<component::Widget>(i);
            widget->update(dt);
        }
    }
}

void Control::treatEventForGame(const sf::Event& event,
                                const sf::Vector2f& ui_position,
                                const sf::Vector2f& map_position)
{
    if(event.type == sf::Event::MouseMoved)
    {
        if(move_)
        {
            sf::View& view = window_->fixed;
            view.move(-(event.mouseMove.x-last_pos.x)*zoom_,
                       -(event.mouseMove.y-last_pos.y)*zoom_);
        }
        else
            emit<event::Mouse::Move>(map_position);
        last_pos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        if(event.mouseButton.button == sf::Mouse::Left)
        {
            move_ = true;
        }
        last_pos = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        if(event.mouseButton.button != sf::Mouse::Right)
        {
            move_ = false;
        }

        emit<event::Mouse::Click>(map_position, event.mouseButton.button);
    }
    else if(event.type == sf::Event::MouseWheelMoved)
    {
        sf::View& view = window_->fixed;
        if(event.mouseWheel.delta == -1)
        {
            view.zoom(1.1);
            zoom_ *= 1.1;
        }
        else
        {
            view.zoom(0.9);
            zoom_ *= 0.9;
        }
    }
}

void Control::release()
{
}

void Control::receive(const event::Game::New& UNUSED(event))
{
    clear_ui();
    playing_ = false;
    gui_.assign<component::Image>(event_manager_,
                                  sf::Vector2f(500, 110), "logo.png");
    gui_.assign<component::Label>(event_manager_,
                                  sf::Vector2f(400, 260), L"Seed :");
    gui_.assign<component::SeedInput>(event_manager_,
                                      sf::Vector2f(500, 260));
    gui_.assign<component::Label>(event_manager_,
                                  sf::Vector2f(400, 310), L"Color :");
    gui_.assign<component::ColorPicker>(event_manager_,
                                        sf::Vector2f(500, 410));
    gui_.assign<component::Button>(event_manager_,
                                   sf::Vector2f(800, 510),
                                   "Play !",
                                   kLoad);
    gui_.assign<component::Button>(event_manager_,
                                   sf::Vector2f(200, 510),
                                   "Quit !",
                                   kQuit);
    gui_.assign<component::Label>(event_manager_,
                                  sf::Vector2f(800, 30),
                                  L"Version 2014.A",
                                  sf::Color(200, 20, 30, 150),
                                  20);
}

void Control::receive(const event::Game::Start& UNUSED(event))
{
    clear_ui();
    playing_ = true;
}

void Control::receive(const event::UI::Pause& event)
{
    clear_ui();
    if(event.Resume)
    {
        gui_.assign<component::Button>(event_manager_,
                                       sf::Vector2f(300, 510),
                                       "Resume", kResume);
    }
    gui_.assign<component::Button>(event_manager_,
                                   sf::Vector2f(500, 510),
                                   "Exit", kQuit);
    gui_.assign<component::Button>(event_manager_,
                                   sf::Vector2f(700, 510),
                                   "New game", kNew);
}

void Control::receive(const event::UI::Resume& event)
{
}

void Control::receive(const event::Game::End& event)
{
    emit<event::UI::Pause>(false);
    if(event.PlayerWin)
    {
        gui_.assign<component::Label>(event_manager_,
                                  sf::Vector2f(280, 310),
                                  L"YOU WIN",
                                  sf::Color(20, 200, 20),
                                  120);
    }
    else
    {
        gui_.assign<component::Label>(event_manager_,
                                  sf::Vector2f(200, 310),
                                  L"GAME OVER",
                                  sf::Color(200, 20, 20),
                                  120);
    }
}

void Control::clear_ui()
{
    uint32_t size = gui_.count<component::Widget>();
    for(uint32_t i=0;i<size;++i)
        gui_.remove<component::Widget>(0);
    size = gui_.count<component::Widget>();
    gui_.destroy();
    gui_ = createEntity();
}

void Control::receive(const event::UI::Button& event)
{
    clear_ui();

    if(event.ID == kQuit)
    {
        run_ = false;
    }
    else if(event.ID == kLoad)
    {
        clear_ui();
        gui_.assign<component::Label>(event_manager_,
                                      sf::Vector2f(470, 260),
                                      L"Loading ...");
        at<event::Game::Build>(std::chrono::seconds(1), 2, seed_, color_);
    }
    else if(event.ID == kResume)
        emit<event::UI::Resume>();
    else if(event.ID == kNew)
    {
        emit<event::Game::New>();
    }
}

void Control::receive(const event::UI::ColorPicker &event)
{
    color_ = event.Color;
    math::Random rng;
    emit<event::Particle::Fireworks>(sf::Vector2f(rng.next(800)+100,
                                                  rng.next(400)+100), color_);
}

void Control::receive(const event::UI::SelectSeed &event)
{
   seed_ = event.Graine;
}

}
}
