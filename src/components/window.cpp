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
#include <SFML/OpenGL.hpp>

#include "window.h"
#include "common/unused.h"
#include "common/events.h"
#include "math/math.h"
#include "ecs/manager.h"
#include "world.h"
#include <X11/Xlib.h>
//#include "physic/blob/blobcollective.h"

namespace beblob
{
namespace component
{

Window::Window(uint32_t width, uint32_t height)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    window_ = std::shared_ptr<sf::RenderWindow>(
                new sf::RenderWindow(sf::VideoMode(width, height),
                                     "BeBlob",
                                     sf::Style::Close, settings));
    window_.get()->setFramerateLimit(60);

    x = y = 0;
    fixed = window_->getView();
    base = window_->getDefaultView();
    window_->setView(fixed);
}

void Window::setCenter(const sf::Vector2f& center)
{
    fixed.setCenter(center);
}

void Window::uiView()
{
    window_->setView(base);
}

void Window::defaultView()
{
    window_->setView(window_->getDefaultView());
}

void Window::updateView()
{
    window_->setView(fixed);
}

sf::Vector2f Window::toMap(const sf::Vector2f& point)
{
    uiView();
    sf::Vector2i m = window_->mapCoordsToPixel(point);
    updateView();
    sf::Vector2f o =  window_->mapPixelToCoords(m);
    return o;
}


sf::Vector2f Window::getMousePosition() const
{
    return window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
}

Window::~Window()
{
    window_->close();
}

}
}
