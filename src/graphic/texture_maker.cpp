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
#include <utility>
#include <iostream>

#include "texture_maker.h"
#include "common/unused.h"
#include "math/vector.h"

namespace beblob
{
namespace procedural
{

sf::Image TextureMaker::pop()
{
    sf::Image image;
    image.create(width_, height_, sf::Color(0, 0, 0));
    for(uint32_t i=0;i<width_;++i)
    {
        for(uint32_t j=0;j<height_;++j)
        {
            //float xx = float(i) / float(width_);
            //float yy = float(j) / float(height_);
            //float f = procedural::scaled_octave_noise_2d(8, 0.9, 1, .5, 1, xx, yy);
            //sf::Color color = getColor(i, j);
            //image.setPixel(i, j, color);
        }
    }
    return image;
}

HorizontalPatternGenerator::HorizontalPatternGenerator(math::Random::Seed seed,
                                                       uint32_t row) :
    PatternGenerator(seed), row_(row)
{
}

sf::Color HorizontalPatternGenerator::color(uint32_t UNUSED(x), uint32_t y)
{
    uint32_t index = y / row_;
    while(Colors_.size() < index+1)
    {
        Colors_.push_back(nextColor());
    }
    return Colors_[index];
}

VerticalPatternGenerator::VerticalPatternGenerator(math::Random::Seed seed,
                                                   uint32_t column) :
    PatternGenerator(seed), column_(column)
{
}

sf::Color VerticalPatternGenerator::color(uint32_t x, uint32_t UNUSED(y))
{
    uint32_t index = x / column_;
    while(Colors_.size() < index+1)
    {
        //@@@
        Colors_.push_back(ChartColors_->next());
    }
    return Colors_[index];
}


DiagonalPatternGenerator::DiagonalPatternGenerator(math::Random::Seed seed,
                                                   uint32_t thickness,
                                                   uint32_t degree) :
    PatternGenerator(seed),
    thickness_(thickness), degree_(degree)
{
}

sf::Color DiagonalPatternGenerator::color(uint32_t x, uint32_t y)
{
    static float angle = degree_/180.0*M_PI;
    float s = - sin(angle);
    float c = - cos(angle);
    sf::Vector2f start(s, c);
    sf::Vector2f end(0, 0);
    sf::Vector2f point(y, x);
    float d = math::distance_to_Line(start, end, point);

    uint32_t index = 0;
    uint32_t distance = floor(d);
    index = distance / thickness_;

    while(Colors_.size() < index+1)
    {
        Colors_.push_back(ChartColors_->next());
    }
    return Colors_[index];
}

RadialPatternGenerator::RadialPatternGenerator(math::Random::Seed seed, uint32_t thickness, const sf::Vector2f& center) :
    PatternGenerator(seed),
    center_(center),
    thickness_(thickness)
{
}

sf::Color RadialPatternGenerator::color(uint32_t x, uint32_t y)
{
    uint32_t distance = math::distance<uint32_t>(sf::Vector2u(width_*center_.x, height_*center_.y), sf::Vector2u(x, y));
    uint32_t index = distance / thickness_;
    while(Colors_.size() < index+1)
    {
        // @@@
        Colors_.push_back(ChartColors_->next());
    }
    return Colors_[index];
}


NoisePatternGenerator::NoisePatternGenerator(math::Random::Seed seed) :
    PatternGenerator(seed)
{
}

sf::Color NoisePatternGenerator::color(uint32_t x, uint32_t y)
{
    //float xx = float(x) / float(width_);
    //float yy = float(y) / float(height_);
    float f = 1;
    return sf::Color(255*f, 255*f, 255*f, 255*f);
}

}
}
