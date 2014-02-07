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

#ifndef BEBLOB_PROCEDURAL_TEXTURE_H
#define BEBLOB_PROCEDURAL_TEXTURE_H

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>

#include "math/random.h"
#include "graphic/chart_colors.h"

namespace beblob
{
namespace procedural
{

struct PatternGenerator
{
    PatternGenerator(math::Random::Seed seed)
    {
       seed_ = seed;
       rng_.setSeed(seed);
    }
    virtual sf::Color color(uint32_t x, uint32_t y) = 0;
    template <typename P, typename ... Args>
    void chartColors(Args && ... args)
    {
        if(ChartColors_) ChartColors_.reset();
        ChartColors_ = std::make_shared<ChartColors<P>>(seed_, std::forward<Args>(args) ...);
    }
    sf::Color nextColor()
    {
        if(!ChartColors_) return sf::Color();
        return ChartColors_->next();
    }
    math::Random::Seed seed_;
    uint32_t width_;
    uint32_t height_;
    math::Random rng_;
    std::shared_ptr<BaseChartColors> ChartColors_;
    std::vector<sf::Color> Colors_;
};

class TextureMaker
{
public:
    struct Layer
    {
        std::shared_ptr<PatternGenerator> pattern_;
        math::Random::Seed seed_;
        template <typename M, typename ... Args>
        void pattern(Args && ... args)
        {
            if(pattern_) pattern_.reset();
            pattern_ = std::make_shared<M>(seed_, std::forward<Args>(args) ...);
            pattern_->width_ = width_;
            pattern_->height_ = height_;
        }
        template <typename P, typename ... Args>
        void chartColors(Args && ... args)
        {
            if(!pattern_) return;
            pattern_->chartColors<P>(std::forward<Args>(args) ...);
        }
        sf::Color color(uint32_t i, uint32_t j)
        {
            if(!pattern_) return sf::Color();
            return pattern_->color(i, j);
        }
        uint32_t width_;
        uint32_t height_;
    };

    inline sf::Color getColor(uint32_t x, uint32_t y)
    {
        int32_t r, g, b, a;
        r = g = b = a = 0;
        a = 255;
        for(auto l : layers_)
        {
            sf::Color c = l.color(x, y);
            r += c.r;
            g += c.g;
            b += c.b;
            //if(c.a != 255)
              //  a = c.a;
        }
        r /= layers_.size();
        g /= layers_.size();
        b /= layers_.size();
        //a /= layers_.size();
        return sf::Color(r, g, b);
    }

    Layer& operator()(uint32_t index)
    {
        while(index >= layers_.size())
        {
            layers_.push_back(Layer());
            layers_.back().seed_ = seed_;
            layers_.back().width_ = width_;
            layers_.back().height_ = height_;
        }
        return layers_[index];
    }

    TextureMaker(math::Random::Seed seed,
                 uint32_t width,
                 uint32_t height) :
        width_(width),
        height_(height),
        seed_(seed)
    {
    }

    sf::Image pop();

private:
    uint32_t width_;
    uint32_t height_;
   math::Random::Seed seed_;
    std::vector<Layer> layers_;
};

struct HorizontalPatternGenerator : public PatternGenerator
{
    HorizontalPatternGenerator(math::Random::Seed seed, uint32_t row);
    sf::Color color(uint32_t x, uint32_t y);
    uint32_t row_;
};

struct VerticalPatternGenerator : public PatternGenerator
{
    VerticalPatternGenerator(math::Random::Seed seed, uint32_t column);
    sf::Color color(uint32_t x, uint32_t y);
    uint32_t column_;
};

struct DiagonalPatternGenerator : public PatternGenerator
{
    DiagonalPatternGenerator(math::Random::Seed seed, uint32_t thickness, uint32_t degree);
    sf::Color color(uint32_t x, uint32_t y);
    uint32_t thickness_;
    uint32_t degree_;
};

struct RadialPatternGenerator : public PatternGenerator
{
    RadialPatternGenerator(math::Random::Seed seed, uint32_t thickness, const sf::Vector2f& center);
    sf::Color color(uint32_t x, uint32_t y);
    sf::Vector2f center_;
    uint32_t thickness_;
};

struct NoisePatternGenerator : public PatternGenerator
{
    NoisePatternGenerator(math::Random::Seed seed);
    sf::Color color(uint32_t x, uint32_t y);
};

}
}

#endif // BEBLOB_PROCEDURAL_TEXTURE_H
