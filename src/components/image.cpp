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

#include "image.h"
#include "common/events.h"

namespace beblob
{
namespace component
{

Image::Image(ecs::EventManager::Ptr event,
             const sf::Vector2f &position,
             const std::string& image)
    : Widget(event)
    , speed_(0)
{
    setPosition(position);
    sf::ZipStream zip = {"blob.pack"};
    zip.open(image);
    texture_.loadFromStream(zip);
    setSize(sf::Vector2f(texture_.copyToImage().getSize().x,
    texture_.copyToImage().getSize().y)
                         );
    rectangle(sf::Color(255, 255, 255), true, texture_);
}

void Image::update(double dt)
{
    setRotation(getRotation()+dt*speed_);
}

}
}
