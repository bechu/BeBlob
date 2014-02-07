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

#include "flower.h"
#include "math/bezier.h"
#include "sf/curve.h"
#include "math/range.h"
#include "graphic/chart_colors.h"
#include "world.h"

namespace beblob
{
namespace component
{

std::vector<std::shared_ptr<sf::RenderTexture>> Flower::textures_;
std::vector<sf::Color> Flower::Colors;
std::vector<float> Flower::f;

math::Random Flower::rng_;
int Flower::w;
int Flower::h;
int Flower::w2;
int Flower::h2;

void Flower::prepare(math::Random::Seed seed)
{
    textures_.clear();
    rng_.setSeed(seed);
    //procedural::ChartColors<procedural::RandomMixColorGenerator> p(rng_.next(seed));
    procedural::ChartColors<procedural::GoldenRatioGradientColorGenerator> p(rng_.next(seed));
    //make(0).chartColors<procedural::GoldenRatioGradientColorGenerator>(event.Graine);
    Colors = p.pickColors(2);

    // precompute values for the flower
    f.resize(2);
    std::generate(f.begin(), f.end(), [&] { return rng_.nextFloat(); });
    w = kSize + rng_.next(10);
    h = kSize + rng_.next(10);
    w2 = 15 + rng_.next(w*0.75);
    h2 = 15 + rng_.next(h*0.75);
}

Flower::Flower()
{
    last_q = 9;
    current_ = textures_[0];
}

void Flower::compute(int q)
{
    float percent = double(q) / double(World::Cell::kDefaultFood);
    current_ = textures_[percent*(kMaxState-1)];
}

Flower::Petal::Petal(float width, float height)
{
    points_ = math::Bezier::compute(20,
                                    sf::Vector2f(0, 0),
                                    sf::Vector2f(-width, height),
                                    sf::Vector2f(width, height),
                                    sf::Vector2f(0, 0));
}

std::vector<sf::Vector2f> Flower::Petal::get(unsigned int width,
                                             unsigned int height,
                                             unsigned int rotation)
{
    float angle = rotation/180.0*M_PI;
    float s = sin(angle);
    float c = cos(angle);
    std::vector<sf::Vector2f> points;
    for(auto point : points_)
    {
        sf::Vector2f p (point.x*width, point.y*height);
        sf::Vector2f p2;
        p2.x = p.x * c + p.y * s;
        p2.y = -p.x * s + p.y * c;
        points.push_back(p2);
    }
    return points;
}

void Flower::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    sf::Sprite sprite(current_.lock()->getTexture());
    sprite.setOrigin(kSize/2, kSize/2);
    target.draw(sprite, states);
}

void Flower::createTexture(int state, math::Random::Seed seed, Flower::Draw& draw)
{
    constexpr int angles[] = {0, 45, 90, 135, 180, 225, 270, 315, 360};

    // take some Colors
    sf::Vector2f center(kSize/2, kSize/2);

    //    for(auto state : math::Range(0, kMaxState))
    {
        float percent = double(state) / double(kMaxState);

        Petal petal(0.05+f[0]*0.5, 0.1+f[1]*0.5);
        for(auto angle : angles)
        {
            if(angle >= 360.0*percent) break;
            std::vector<sf::Vector2f> points = petal.get(w, h, angle);
            for(auto point : points)
            {
                draw.inside.push_back(sf::Vertex(point+center, Colors[0]));
                draw.inside.push_back(sf::Vertex(center, sf::Color()));
            }
        }

        for(auto angle : angles)
        {
            if(angle >= 360.0*percent) break;
            std::vector<sf::Vector2f> points = petal.get(w2, h2, angle+45);
            for(auto point : points)
            {
                draw.outside.push_back(sf::Vertex(center, Colors[1]));
                draw.outside.push_back(sf::Vertex(point+center, sf::Color()));
            }
        }

    }
}

std::shared_ptr<sf::RenderTexture> Flower::drawTexture(const Flower::Draw& flow)
{
    textures_.push_back(std::make_shared<sf::RenderTexture>());
    std::shared_ptr<sf::RenderTexture> texture = textures_.back();
    texture->create(kSize, kSize);
    texture->clear(sf::Color(0, 0, 30, 0));
    texture->draw(&flow.inside[0], flow.inside.size(), sf::TrianglesStrip);
    texture->draw(&flow.outside[0], flow.inside.size(), sf::TrianglesStrip);
    texture->setSmooth(true);
    return texture;
}

void Flower::clear()
{
    f.clear();
    textures_.clear();
    Colors.clear();
}

}
}
