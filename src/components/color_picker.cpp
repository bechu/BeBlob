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

#include "color_picker.h"
#include "common/events.h"

namespace beblob
{
namespace component
{

ColorPicker::ColorPicker(ecs::EventManager::Ptr event, const sf::Vector2f &position)
    : Widget(event)
{
    setPosition(position);
    setSize(sf::Vector2f(200, 200));
    sf::ZipStream zip = {"blob.pack"};
    zip.open("ColorPicker.png");
    texture_.loadFromStream(zip);
    rectangle(sf::Color(255, 255, 255), true, texture_);
}

void ColorPicker::onClick(const sf::Vector2f& ui, const sf::Vector2f& map)
{
    sf::Vector2f rel = ui - getPosition() + sf::Vector2f(getSize().x/2, getSize().y/2);
    sf::Vector2u size = texture_.copyToImage().getSize();
    sf::Color color = texture_.copyToImage().getPixel(rel.x*(size.x/getSize().x),
                                                      rel.y*(size.y/getSize().y));
    events_->emit<event::UI::ColorPicker>(color);
}

}
}
