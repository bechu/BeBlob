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

#include "home.h"

namespace beblob
{
namespace component
{

void Home::compute(component::LandSolid& land, const sf::Color &color)
{
    shape_.clear();
    outline_.clear();
    shape_.setPrimitiveType(sf::TrianglesFan);
    for(uint32_t i=0;i<land.points.size();i++)
    {
        sf::Vertex vertex;
        vertex.position = land.points[i];
        vertex.color = color;
        shape_.append(vertex);
        outline_.add(vertex.position, 3, sf::Color::White);
    }
    shape_.append(shape_[0]);
    outline_.add(shape_[0].position, 3, sf::Color::White);
}

void Home::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(shape_, states);
    target.draw(outline_, states);
}

}
}
