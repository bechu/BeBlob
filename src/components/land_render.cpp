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
#include <random>

#include "graphic/chart_colors.h"
#include "math/random.h"
#include "math/vector.h"
#include "sf/hsl.h"
#include "land_render.h"
#include "land_physic.h"

namespace beblob
{

namespace component
{

sf::Texture LandRender::texture;

LandRender::LandRender()
{
}

void LandRender::compute(LandSolid& land)
{
    //static int red = 0;
    static std::map<uint8_t, sf::Color> toto;
    static procedural::ChartColors<procedural::UniformColorGenerator> pal(989);
    //text = &texture;
    vertices_.clear();
    vertices_.setPrimitiveType(sf::TrianglesFan);
    vertices_.setPrimitiveType(sf::TrianglesStrip);
    sf::Color color;
    color = pal.next();
    sf::HSL hsl(color);
    hsl.l-=.1;
    sf::Color lum2 = hsl();
    sf::Vector2f center(land.x+land.width/2, land.y+land.height/2);
    for(uint32_t i=0;i<land.points.size();i++)
    {
        sf::Vertex vertex;
        vertex.position = land.points[i];
        //vertex.texCoords.x = land.points[i].x;
        //vertex.texCoords.y = land.points[i].y;
//        vertex.color = color;
//        vertex.color = sf::Color(255, 255, 255);
        vertex.color = color;
        //vertex.color = sf::Color::White;
        vertices_.append(vertex);
        vertex.position = center;
        //vertex.texCoords.x = center.x;
        //vertex.texCoords.y = center.y;
        vertex.color = lum2;
        //vertex.color = sf::Color::White;
        vertices_.append(vertex);
    }
    vertices_.append(vertices_[0]);
}

bool LandRender::isInside(const sf::FloatRect& rect)
{
    for(uint32_t i=0;i<vertices_.getVertexCount();++i)
        if(rect.contains(vertices_[i].position) == true)
            return true;
    return false;
}

void LandRender::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    //states.texture = text;
    target.draw(vertices_, states);
}

}
}
