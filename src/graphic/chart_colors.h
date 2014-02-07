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

#ifndef BEBLOB_ChartColors_H
#define BEBLOB_ChartColors_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <array>
#include <SFML/Graphics/Color.hpp>

#include "math/random.h"

namespace beblob
{
namespace procedural
{

struct ColorGenerator
{
    ColorGenerator(math::Random::Seed seed, uint32_t limit) : limit_(limit)
    {
        rng_.setSeed(seed);
    }
    sf::Color next()
    {
        while(index_ >= colors_.size())
            colors_.push_back(nextColor());
        if(limit_ != 0)
            if(index_ >= limit_)
                index_ = 0;
        return colors_[index_++];
    }
    virtual sf::Color nextColor() = 0;
    math::Random rng_;
    unsigned int index_ = 0;
    unsigned int limit_ = 0;
    std::vector<sf::Color> colors_;
};

struct BaseChartColors
{
    virtual sf::Color next() = 0;
    virtual sf::Color operator() () = 0;
    bool gradient_ = false;
};

template <typename P>
class ChartColors : public BaseChartColors
{
public:
    template <typename ... Args>
    ChartColors(math::Random::Seed seed, Args && ... args)
    {
        generator_ = std::make_shared<P>(seed, std::forward<Args>(args) ...);
    }
    inline sf::Color next()
    {
        return generator_->next();
    }
    sf::Color operator() ()
    {
        return next();
    }
    std::vector<sf::Color> pickColors(uint32_t count)
    {
        std::vector<sf::Color> colors;
        while(colors.size() < count)
            colors.push_back(next());
        return colors;
    }
private:
    std::shared_ptr<P> generator_;
};

struct PresetColorGenerator : public ColorGenerator
{
    PresetColorGenerator(math::Random::Seed seed, std::vector<sf::Color> colors, uint32_t limit=0) :
        ColorGenerator(seed, limit)
    {
        colors_ = colors;
        limit_ = colors_.size();
    }
    sf::Color nextColor()
    {
        return sf::Color();
    }
};

struct UniformColorGenerator : public ColorGenerator
{
    UniformColorGenerator(math::Random::Seed seed, uint32_t limit=0) :
        ColorGenerator(seed, limit)
    {}
    sf::Color nextColor();
};

struct GradientColorGenerator : public ColorGenerator
{
    GradientColorGenerator(math::Random::Seed seed, uint32_t limit, const std::map<float, sf::Color>& colors) :
        ColorGenerator(seed, limit), mColors(colors)
    {
    }
    typedef std::map<float, sf::Color>	Gradient;
    sf::Color nextColor();
    std::map<float, sf::Color>	mColors;
};

struct RandomWalkColorGenerator : public ColorGenerator
{
    RandomWalkColorGenerator(math::Random::Seed seed, uint32_t limit=0);
    sf::Color nextColor();
    sf::Color n_;
    float max_;
    float min_;
};

struct OffsetColorGenerator : public ColorGenerator
{
    OffsetColorGenerator(math::Random::Seed seed, uint32_t limit=0);
    sf::Color nextColor();
    sf::Color color_;
    float maxRange_ = 0;
};


struct RandomMixColorGenerator : public ColorGenerator
{
    RandomMixColorGenerator(math::Random::Seed seed, uint32_t limit=0);
    sf::Color nextColor();
    std::vector<sf::Color> colors_;
    float greyControl_ = 0;
};

struct GoldenRatioRainbowColorGenerator : public ColorGenerator
{
    GoldenRatioRainbowColorGenerator(math::Random::Seed seed, uint32_t limit=0);
    sf::Color nextColor();
    float currentHue_;
    float saturation_;
    float luminance_;
};

struct GoldenRatioGradientColorGenerator : public ColorGenerator
{
    GoldenRatioGradientColorGenerator(math::Random::Seed seed, uint32_t limit=0);
    sf::Color nextColor();
    float saturation_;
    float luminance_;
    float currentHue_;
    std::vector<sf::Color> colors_;
};

}
}

#endif // BEBLOB_ChartColors_H
