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

#ifndef BEBLOB_LIGHT_MANAGER_H
#define BEBLOB_LIGHT_MANAGER_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "components/light.h"

namespace beblob
{

class LightManager
{
public:
    LightManager();
    virtual ~LightManager();

    void load(int width, int height);
    void addObstacle(const sf::Vector2f &pt1, const sf::Vector2f &pt2);

    template <typename C>
    std::shared_ptr<C> addLight();

    void addLight(component::Light::Ptr light);

    void deleteLight(component::Light::Ptr light);

    void clear();
    void clearObstacles();
    void clearLights();

    void compute();
    void draw(sf::RenderWindow& window);

    inline void setLightSmooth(int level)
    {
        _lightSmooth = level;
    }
private:
    const static int kMininumLightAmbiance = 50;
    component::Light::Obstacle::List _obstacles;
    component::Light::List _lights;
    sf::Shader _blurEffect;
    sf::RenderTexture _render;
    int _lightSmooth;
};

template <typename C>
std::shared_ptr<C> LightManager::addLight()
{
    std::shared_ptr<C> light = std::make_shared<C>();
    _lights.push_back(light);
    return light;
}

inline void LightManager::addLight(component::Light::Ptr light)
{
    _lights.push_back(light);
}

inline void LightManager::clearObstacles()
{
    _obstacles.clear();
}

inline void LightManager::clearLights()
{
    _lights.clear();
}

inline void LightManager::clear()
{
    clearLights();
    clearObstacles();
}

}

#endif // BEBLOB_LIGHT_MANAGER_H
