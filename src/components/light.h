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

#ifndef BEBLOB_LIGHT_H
#define BEBLOB_LIGHT_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <ecs/ecs.h>

namespace beblob
{
namespace component
{

class LightManager;

class Light
        : public ecs::Component<Light>
{
public:
  struct Obstacle
  {
    typedef std::vector<Obstacle> List;
    Obstacle(const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
      pt1 = p1;
      pt2 = p2;
    }
    sf::Vector2f pt1;
    sf::Vector2f pt2;
    // double opacity; // for the futur
  };
  typedef std::shared_ptr<Light> Ptr;
  typedef std::vector<Ptr> List;

  Light();
  virtual void compute(Obstacle::List &obstacles);

  void draw(sf::RenderTarget *App);

  void setIntensity(float intensity);
  void setRadius(float radius);
  void setQuality(int quality);
  void setColor(const sf::Color &color);

  inline void setColor(int r, int g, int b)
  {
    setColor(sf::Color(r, g, b));
  }
  void setPosition(const sf::Vector2f &position);
  inline void setPosition(float x, float y)
  {
    setPosition(sf::Vector2f(x, y));
  }

  void enable();
  void disable();
protected:
  sf::Vector2f Intersect(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3, const sf::Vector2f &p4);
  void addTriangle(const sf::Vector2f &pt1, const sf::Vector2f &B, const Obstacle::List &obstacles);

  bool _active;
  sf::Vector2f _position;
  float _intensity;
  float _radius;
  sf::Color _color;
  int _quality;
  std::vector<sf::VertexArray> _shapes;
};

inline void Light::setIntensity(float intensity)
{
  _intensity = intensity;
}

inline void Light::setRadius(float radius)
{
  _radius = radius;
}

inline void Light::setQuality(int quality)
{
  _quality = quality;
}

inline void Light::setColor(const sf::Color &color)
{
  _color = color;
}

inline void Light::setPosition(const sf::Vector2f &position)
{
  _position = position;
}

inline void Light::enable()
{
  _active = true;
}

inline void Light::disable()
{
  _active = false;
}

class DirectionalLight
        : public Light
{
public:
  DirectionalLight();
  typedef std::shared_ptr<DirectionalLight> Ptr;
  void setAngle(float angle);
  float getAngle();
  void setOpeningAngle(float angle);
  virtual void compute(Obstacle::List &obstacles);
private:
  float _angle;
  float _openingAngle;
};

inline void DirectionalLight::setAngle(float angle)
{
  _angle = angle;
}

inline float DirectionalLight::getAngle()
{
  return _angle;
}

inline void DirectionalLight::setOpeningAngle(float angle)
{
  _openingAngle = angle;
}

}
}

#endif // BEBLOB_LIGHT_H
