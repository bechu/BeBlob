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

#include <cmath>

#include "light_manager.h"

namespace beblob
{

namespace
{
const std::string blurShader=\
        "uniform sampler2D texture;"\
        "uniform float offset;"\
        "void main()"\
        "{"\
        "vec2 offx = vec2(offset, 0.0);"\
        "vec2 offy = vec2(0.0, offset);"\
        "vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) * 1.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy - offx) * 2.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy + offx) * 2.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy - offy) * 2.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy + offy) * 2.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy - offx - offy) * 1.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy - offx + offy) * 1.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy + offx - offy) * 1.0 + "\
        "texture2D(texture, gl_TexCoord[0].xy + offx + offy) * 1.0;"\
        "gl_FragColor =  gl_Color * (pixel / 13.0);"\
        "}";
}

LightManager::LightManager() : _lightSmooth(1)
{
}

LightManager::~LightManager()
{
}

void LightManager::load(int width, int height)
{
    _blurEffect.setParameter("texture", sf::Shader::CurrentTexture);
    _blurEffect.loadFromMemory(blurShader, sf::Shader::Fragment);
    _render.create(width, height);
}

void LightManager::draw(sf::RenderWindow& window)
{
    sf::RenderStates r(sf::BlendMultiply);
    r.shader = &_blurEffect;
    window.draw(sf::Sprite(_render.getTexture()), r);
}

void LightManager::compute()
{
    _render.clear(sf::Color(32,32,32));
    _render.clear(sf::Color());
    _blurEffect.setParameter("offset", static_cast<float>(0.005 * _lightSmooth));

    for(auto &light : _lights)
    {
        light->compute(_obstacles);
        light->draw(&_render);
    }

   // std::vector<sf::Vertex> Wall;
   // for(auto obs : _obstacles)
   // {
   //     Wall.push_back(sf::Vertex(obs.pt1));
   //     Wall.push_back(sf::Vertex(obs.pt2));
   // }

    //_render.draw(&Wall[0], static_cast<unsigned int>(Wall.size()), sf::Lines);

    _render.display();
}


void LightManager::addObstacle(const sf::Vector2f &pt1, const sf::Vector2f &pt2) {
    _obstacles.push_back(component::Light::Obstacle(pt1, pt2));
}

}
