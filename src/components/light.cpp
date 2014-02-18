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

#include <cmath>

#include "math/constants.h"
#include "light.h"

namespace beblob
{
namespace component
{

Light::Light() :
  _active(true),
  _intensity(255),
  _radius(128),
  _quality(16)
{
}

void Light::draw(sf::RenderTarget *App)
{
  for(auto &shape : _shapes)
    App->draw(shape, sf::BlendAdd);
}

void Light::compute(Obstacle::List &obstacles)
{
  _shapes.clear();

  float angle = static_cast<float>((PI*2) / float(_quality));

  for(int i=0;i<_quality;++i)
  {
    addTriangle(
          sf::Vector2f(_radius*cos(float(i)*angle),
                       _radius*sin((float(i)*angle))),
          sf::Vector2f(_radius*cos(float(i+1)*angle),
                       _radius*sin(float(i+1)*angle)),
          obstacles);
  }
}

void Light::addTriangle(const sf::Vector2f &A, const sf::Vector2f &B, const Obstacle::List &obstacles)
{
  sf::Vector2f pt1 = A;
  sf::Vector2f pt2 = B;
  for(Obstacle::List::const_iterator it=obstacles.begin();it!=obstacles.end();++it)
  {
    sf::Vector2f l1(it->pt1.x-_position.x, it->pt1.y-_position.y);
    sf::Vector2f l2(it->pt2.x-_position.x, it->pt2.y-_position.y);

    if(l1.x * l1.x + l1.y * l1.y < _radius * _radius)
    {
      sf::Vector2f i = Intersect(pt1,pt2,sf::Vector2f (0,0),l1);

      if (pt1 != i && pt2 != i)
        if((pt1.x >= i.x && pt2.x <= i.x) || (pt1.x <= i.x && pt2.x >= i.x))
          if((pt1.y >= i.y && pt2.y <= i.y) || (pt1.y <= i.y && pt2.y >= i.y))
            if((l1.y >= 0 && i.y >= 0) || (l1.y <= 0 && i.y <= 0))
              if((l1.x >= 0 && i.x >= 0) || (l1.x <= 0 && i.x <= 0))
              {
                addTriangle(i, pt2, obstacles);
                pt2 = i;
              }
    }
    if(l2.x * l2.x + l2.y * l2.y < _radius * _radius)
    {
      sf::Vector2f i = Intersect(pt1,pt2,sf::Vector2f (0,0),l2);

      if (pt1 != i && pt2 != i)
        if((pt1.x >= i.x && pt2.x <= i.x) || (pt1.x <= i.x && pt2.x >= i.x))
          if((pt1.y >= i.y && pt2.y <= i.y) || (pt1.y <= i.y && pt2.y >= i.y))
            if((l2.y >= 0 && i.y >= 0) || (l2.y <= 0 && i.y <= 0))
              if((l2.x >= 0 && i.x >= 0) || (l2.x <= 0 && i.x <= 0))
              {
                addTriangle(pt1, i, obstacles);
                pt1 = i;
              }
    }

    sf::Vector2f m = Intersect(l1, l2, sf::Vector2f(0,0), pt1);
    sf::Vector2f n = Intersect(l1, l2, sf::Vector2f(0,0), pt2);
    sf::Vector2f o = Intersect(l1, l2, pt1, pt2);

    if((m.x != 0 || m.y != 0) && (n.x != 0 || n.y != 0))
      pt1 = m, pt2 = n;
    else
    {
      if((m.x != 0 || m.y != 0) && (o.x != 0 || o.y != 0))
      {
        addTriangle(m ,o , obstacles);
        pt1 = o;
      }

      if((n.x != 0 || n.y != 0) && (o.x != 0 || o.y != 0))
      {
        addTriangle(o ,n , obstacles);
        pt2 = o;
      }
    }
  }

  float intensity;

  _shapes.push_back(sf::VertexArray ());
  _shapes.back().setPrimitiveType(sf::Triangles);

  _shapes.back().append(sf::Vertex(_position,
                                   sf::Color((int)(_intensity*_color.r/255),
                                             (int)(_intensity*_color.g/255),
                                             (int)(_intensity*_color.b/255))));

  intensity = _intensity-sqrt(pt1.x*pt1.x + pt1.y*pt1.y)*_intensity/_radius;
  _shapes.back().append(sf::Vertex(_position + pt1,
                                   sf::Color((int)(intensity*_color.r/255),
                                             (int)(intensity*_color.g/255),
                                             (int)(intensity*_color.b/255))));

  intensity = _intensity-sqrt(pt2.x*pt2.x + pt2.y*pt2.y)*_intensity/_radius;
  _shapes.back().append(sf::Vertex(_position + pt2,
                                   sf::Color((int)(intensity*_color.r/255),
                                             (int)(intensity*_color.g/255),
                                             (int)(intensity*_color.b/255))));
}

sf::Vector2f Light::Intersect(const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p3, const sf::Vector2f &p4)
{
  float xD1,yD1,xD2,yD2,xD3,yD3;
  float dot,deg,len1,len2;
  float segmentLen1,segmentLen2;
  float ua,div;

  // calculate differences
  xD1=p2.x-p1.x;
  xD2=p4.x-p3.x;
  yD1=p2.y-p1.y;
  yD2=p4.y-p3.y;
  xD3=p1.x-p3.x;
  yD3=p1.y-p3.y;

  // calculate the lengths of the two lines
  len1=sqrt(xD1*xD1+yD1*yD1);
  len2=sqrt(xD2*xD2+yD2*yD2);

  // calculate angle between the two lines.
  dot=(xD1*xD2+yD1*yD2); // dot product
  deg=dot/(len1*len2);

  // if abs(angle)==1 then the lines are parallell,
  // so no intersection is possible
  if(abs(deg)==1) return sf::Vector2f();

  // find intersection Pt between two lines
  sf::Vector2f pt;
  div=yD2*xD1-xD2*yD1;
  if(div == 0) return sf::Vector2f();
  ua=(xD2*yD3-yD2*xD3)/div;
  //ub=(xD1*yD3-yD1*xD3)/div;
  pt.x=p1.x+ua*xD1;
  pt.y=p1.y+ua*yD1;

  // calculate the combined length of the two segments
  // between Pt-p1 and Pt-p2
  xD1=pt.x-p1.x;
  xD2=pt.x-p2.x;
  yD1=pt.y-p1.y;
  yD2=pt.y-p2.y;
  segmentLen1=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

  // calculate the combined length of the two segments
  // between Pt-p3 and Pt-p4
  xD1=pt.x-p3.x;
  xD2=pt.x-p4.x;
  yD1=pt.y-p3.y;
  yD2=pt.y-p4.y;
  segmentLen2=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

  // if the lengths of both sets of segments are the same as
  // the lenghts of the two lines the point is actually
  // on the line segment.

  // if the point isn't on the line, return null
  if(abs(len1-segmentLen1)>0.01 || abs(len2-segmentLen2)>0.01)
    return sf::Vector2f();
  return pt;

}

DirectionalLight::DirectionalLight()
  : Light(), _angle(0), _openingAngle(0)
{
}

void DirectionalLight::compute(Obstacle::List &obstacles)
{
  _shapes.clear();

  float angle     = static_cast<float>(_angle * PI / 180.0f);
  float o_angle   = static_cast<float>(_openingAngle * PI / 180.0f);

  addTriangle(
        sf::Vector2f(_radius*cos( float(angle + o_angle * 0.5) ),
                     _radius*sin( float(angle + o_angle * 0.5) )),
        sf::Vector2f(_radius*cos( float(angle - o_angle * 0.5) ),
                     _radius*sin( float(angle - o_angle * 0.5) )),
        obstacles);
}


}
}

