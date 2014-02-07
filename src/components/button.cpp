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

#include "button.h"
#include "common/events.h"

namespace beblob
{
namespace component
{

Button::Button(ecs::EventManager::Ptr event, const sf::Vector2f &position, const std::string &label, int id) :
    Widget(event),
    id_(id)
{
    setPosition(position);
    setSize(sf::Vector2f(label.size()*20, 30));
    rectangle(sf::Color(200, 0, 0));
    text(sf::Vector2f(20, 0), sf::String(label.c_str()), sf::Color(255, 255, 255));
}

void Button::onClick(const sf::Vector2f& ui, const sf::Vector2f& map)
{
    events_->emit<event::UI::Button>(id_);
}

}
}
