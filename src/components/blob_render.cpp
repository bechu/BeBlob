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

#include "blob_render.h"
#include "world.h"
#include "sf/hsl.h"
#include "math/vector.h"

namespace beblob
{
namespace component
{

int BlobRender::count = 1234;

BlobRender::BlobRender()
{
    rng.setSeed(count * 23);
    count++;
}

void BlobRender::computeShape(BlobSolid& physic, BlobAI& logic)
{
    shape_.clear();
    shape_.setPrimitiveType(sf::Lines);
    shape_.setPrimitiveType(sf::TrianglesStrip);

    sf::HSL hsl(fillColor_);
    hsl.l-=.3;
    sf::Color lum2 = hsl();
    around_shape_.clear();

    for(unsigned int i = 0; i < BlobSolid::kNumPointMasses; i++)
    {
        physic::PointMass& p = physic.getPointMasse(i);
        sf::Vertex v;
        v.position = p.currPos;
        v.color = lum2;
        shape_.append(v);
        shape_.append(sf::Vertex(physic.getMiddle().currPos, fillColor_));
        around_shape_.add(p.currPos, 3, sf::Color(10, 10, 10));
    }
    shape_.append(shape_[0]);
    around_shape_.add(shape_[0].position, 3, sf::Color(10, 10, 10));
}

void BlobRender::computeFace(double dt,
                       BlobSolid& physic,
                       BlobAI& logic,
                       const sf::Vector2f& mouse)
{
    sf::Vector2f orientation(physic.getPointMasse(0).currPos);
    orientation += physic.getPointMasse(1).currPos;
    orientation -= physic.getPointMasse(2).currPos;
    orientation -= physic.getPointMasse(3).currPos;
    sf::Vector2f up(0.0f, 1.0f);
    math::normalize(orientation, 1.0f);
    float angle = std::acos(math::cross(orientation, up));
    if(orientation.x > 0.0f)
        angle = -angle;
    float degs = 360.0f * angle / (2.0f * M_PI);

    change-=dt;
    if(change < 0)
    {
        change = 1000;
        r = rng.nextFloat();
    }

    mouth_.setPosition(physic.getMiddle().currPos);
    mouth_.update(dt, physic, r);
    mouth_.setRotation(degs);

    // blob selected, eyes follow mouse !
    if(logic.select())
    {
        last_mouse_position_ = mouse;
        r = 0.5;
    }

    left_.setPosition(physic.getMiddle().currPos);
    left_.setRotation(degs);
    left_.update(physic, -0.22, last_mouse_position_, r);

    right_.setPosition(physic.getMiddle().currPos);
    right_.setRotation(degs);
    right_.update(physic, 0.22, last_mouse_position_, r);
}

void BlobRender::compute(double dt,
                   BlobSolid& physic,
                   BlobAI &logic,
                   const sf::Vector2f &mouse)
{
    computeShape(physic, logic);
    computeFace(dt, physic, logic, mouse);

    sf::Color color;
    if(logic.state() >= 0.6)
        color = sf::Color(10, 200, 10);
    else if(logic.state() >= 0.4)
        color = sf::Color(210, 200, 10);
    else if(logic.state() >= 0.0)
        color = sf::Color(100, 0, 0);

    ring_.clear();
    state_.clear();
    float q = 30;
    float angleStep = 2.0 * M_PI / q;
    for(float i=0;i<=q+1;i++)
    {
        sf::Vector2f v;
        v.x = physic.getPosition().x +
                BlobSolid::kDefaultRadius* 1.6 * sin(i*angleStep);
        v.y = physic.getPosition().y +
                BlobSolid::kDefaultRadius* 1.6 * cos(i*angleStep);
        int max = q * logic.state();
        if(logic.select())
            ring_.add(v, 10, sf::Color(80, 80, 80));
        if(logic.focus())
        {
            if(i <= max)
            {
                state_.add(v, 6, color);
            }
        }
    }
}

void BlobRender::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(shape_, states);
    target.draw(around_shape_, states);
    target.draw(ring_, states);
    target.draw(mouth_, states);
    target.draw(left_, states);
    target.draw(right_, states);
    target.draw(state_, states);
}

void BlobRender::Arc::compute(const sf::Vector2f& p,
                              float aRadius,
                              float startDeg,
                              float numDegs,
                              bool filled,
                              const sf::Color& color)
{
    sf::VertexArray vertices;
    if(filled)
        vertices.setPrimitiveType(sf::TrianglesFan);
    else
        vertices.setPrimitiveType(sf::LinesStrip);
    int count = 60;
    for(int i=count*startDeg;i<=count*numDegs;++i)
    {
        float angle = i * 2 * M_PI / count - M_PI / 2;
        float x = p.x + std::cos(angle) * aRadius;
        float y = p.y + std::sin(angle) * aRadius;
        sf::Vertex v;
        v.position = sf::Vector2f(x, y);
        v.color = color;
        vertices.append(v);
    }
    vertex_.push_back(vertices);
}

void BlobRender::Arc::draw(sf::RenderTarget &target,
                           sf::RenderStates states) const
{
    states.transform *= getTransform();
    for(auto vs : vertex_)
        target.draw(vs, states);
}

void BlobRender::Mouth::update(double dt, BlobSolid& physic, float r)
{
    clear();
    compute(sf::Vector2f(0, -0.3f*physic.getRadius()),
            physic.getRadius()*0.35f, 0.25, 0.75, r>0.5);
}

void BlobRender::Eye::update(BlobSolid& physic,
                             float offset,
                             const sf::Vector2f& mouse,
                             float r)
{
    clear();
    if(r < 0.1)
        ooh(physic, offset, mouse);
    else if(r < 0.3)
        blink(physic, offset, mouse);
    else
        open(physic, offset, mouse);
}

void BlobRender::Eye::open(BlobSolid& physic,
                           float offset,
                           const sf::Vector2f &mouse)
{
    float radius = physic.getRadius() * 0.20f;
    compute(sf::Vector2f(offset*physic.getRadius(), -0.6f*physic.getRadius()),
            radius, 0, 1, false);

    float x = offset * physic.getRadius();
    float y = -0.6f*physic.getRadius();
    float deltaY = mouse.y - (y+getPosition().y);
    float deltaX = mouse.x - (x+getPosition().x);
    float angle = atan2(deltaY, deltaX) - (getRotation()/180.0*M_PI);
    x += std::cos(angle) * radius * 0.5;
    y += std::sin(angle) * radius * 0.5;
    compute(sf::Vector2f(x, y),
            physic.getRadius()*0.1f, 0, 1, true, sf::Color(0, 0, 0));
}

void BlobRender::Eye::blink(BlobSolid& physic,
                            float offset,
                            const sf::Vector2f &mouse)
{
    float radius = physic.getRadius() * 0.20f;
    compute(sf::Vector2f(offset*physic.getRadius(), -0.6f*physic.getRadius()),
            radius, 0, 1, false);
    sf::Vector2f a(offset*physic.getRadius()-radius, -0.6f*physic.getRadius());
    sf::Vector2f b(offset*physic.getRadius()+radius, -0.6f*physic.getRadius());
    sf::VertexArray va;
    va.append(sf::Vertex(a, sf::Color()));
    va.append(sf::Vertex(b, sf::Color()));
    va.setPrimitiveType(sf::Lines);
    vertex_.push_back(va);
}

void BlobRender::Eye::ooh(BlobSolid& physic,
                          float offset,
                          const sf::Vector2f &mouse)
{
    static bool left = true;
    float radius = physic.getRadius() * 0.20f;
    sf::Vector2f a;
    sf::Vector2f b;
    sf::Vector2f c;
    sf::Vector2f d;
    if(left)
    {
        a = sf::Vector2f(offset*physic.getRadius()-radius,
                         -0.6f*physic.getRadius()-radius);
        b = sf::Vector2f(offset*physic.getRadius()+radius,
                         -0.6f*physic.getRadius());
        c = sf::Vector2f(offset*physic.getRadius()-radius,
                         -0.6f*physic.getRadius()+radius);
        d = sf::Vector2f(offset*physic.getRadius()+radius,
                         -0.6f*physic.getRadius());
    }
    else
    {
        a = sf::Vector2f(offset*physic.getRadius()-radius,
                         -0.6f*physic.getRadius());
        b = sf::Vector2f(offset*physic.getRadius()+radius,
                         -0.6f*physic.getRadius()-radius);
        c = sf::Vector2f(offset*physic.getRadius()-radius,
                         -0.6f*physic.getRadius());
        d = sf::Vector2f(offset*physic.getRadius()+radius,
                         -0.6f*physic.getRadius()+radius);
    }
    sf::VertexArray va;
    va.append(sf::Vertex(a, sf::Color()));
    va.append(sf::Vertex(b, sf::Color()));
    va.append(sf::Vertex(c, sf::Color()));
    va.append(sf::Vertex(d, sf::Color()));
    va.setPrimitiveType(sf::Lines);
    vertex_.push_back(va);
    left = !left;
}

}
}
