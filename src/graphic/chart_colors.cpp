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

#include <random>
#include <cassert>

#include "chart_colors.h"
#include "sf/hsl.h"
#include "math/random.h"
#include "math/math.h"

namespace beblob
{
namespace procedural
{

//float saturation[] = {1.0f, 0.7f, 0.3f};
//float luminance[] = {0.45f, 0.7f, 0.4f};

//Colors ColorParameter::mixColors()
//{
//    sf::Color mixColor1[] =
//    {
//        sf::Color(255, 0, 33),
//        sf::Color(255, 100, 33),
//        sf::Color(128, 0, 33),
//    };
//
//    sf::Color mixColor2[] =
//    {
//        sf::Color(255, 255, 0),
//        sf::Color(255, 255, 100),
//        sf::Color(255, 128, 0),
//    };
//
//    sf::Color mixColor3[] =
//    {
//        sf::Color(0, 100, 255),
//        sf::Color(100, 150, 255),
//        sf::Color(0, 80, 128),
//    };
//    int version = 0;
//    return Colors({mixColor1[version], mixColor2[version], mixColor3[version]});
//}
//
//float ColorParameter::greyValue()
//{
//    return 0.2f;
//}
//
//sf::Color MonoColorGenerator::next()
//{
//    return color_;
//}
//
//sf::Color PredefineColorGenerator::next()
//{
//    assert(Colors_.size()>0);
//    if(index_ >= Colors_.size())
//        index_ = 0;
//    return Colors_[index_++];
//}
//

////////////////////// UNIFORM COLOR
sf::Color UniformColorGenerator::nextColor()
{
    return sf::Color(rng_.nextByte(),
                     rng_.nextByte(),
                     rng_.nextByte());
}

////////////////////// RANDOMWALKCOLOR
RandomWalkColorGenerator::RandomWalkColorGenerator(math::Random::Seed seed,
                                                   uint32_t limit) :
    ColorGenerator(seed, limit)
{
    n_ = sf::Color(rng_.nextByte(), rng_.nextByte(), rng_.nextByte());
    min_ = rng_.nextFloat()*2;
    max_ = min_ + rng_.nextFloat()*7.0;
}

sf::Color RandomWalkColorGenerator::nextColor()
{
    float range = max_ - min_;

    int rSign = rng_.next(2) % 2 == 0 ? 1 : -1;
    int gSign = rng_.next(2) % 2 == 0 ? 1 : -1;
    int bSign = rng_.next(2) % 2 == 0 ? 1 : -1;

    sf::Color newColor = sf::Color(
                static_cast<unsigned char>(n_.r * rSign * (min_ + rng_.nextDouble() * range)),
                static_cast<unsigned char>(n_.g * gSign * (min_ + rng_.nextDouble() * range)),
                static_cast<unsigned char>(n_.b * bSign * (min_ + rng_.nextDouble() * range)));

    n_ = newColor;

    return newColor;
}

OffsetColorGenerator::OffsetColorGenerator(math::Random::Seed seed,
                                           uint32_t limit) :
    ColorGenerator(seed, limit)
{
    color_ = sf::Color(rng_.nextByte(), rng_.nextByte(), rng_.nextByte());
    maxRange_ = rng_.nextFloat() * 100;
}

sf::Color OffsetColorGenerator::nextColor()
{
    color_ = sf::Color(
                static_cast<unsigned char>((static_cast<float>(color_.r) + rng_.nextFloat() * 2 * maxRange_ - maxRange_)),
                static_cast<unsigned char>((static_cast<float>(color_.g) + rng_.nextFloat() * 2 * maxRange_ - maxRange_)),
                static_cast<unsigned char>((static_cast<float>(color_.b) + rng_.nextFloat() * 2 * maxRange_ - maxRange_)));
    return color_;
}

RandomMixColorGenerator::RandomMixColorGenerator(math::Random::Seed seed,
                                                 uint32_t limit) :
    ColorGenerator(seed, limit)
{
    sf::Color mixColor1[] =
    {
        sf::Color(255, 0, 33),
        sf::Color(255, 100, 33),
        sf::Color(128, 0, 33),
    };

    sf::Color mixColor2[] =
    {
        sf::Color(255, 255, 0),
        sf::Color(255, 255, 100),
        sf::Color(255, 128, 0),
    };

    sf::Color mixColor3[] =
    {
        sf::Color(0, 100, 255),
        sf::Color(100, 150, 255),
        sf::Color(0, 80, 128),
    };
    int version = rng_.next(2);
    colors_ = std::vector<sf::Color>({mixColor1[version], mixColor2[version], mixColor3[version]});
    greyControl_ = rng_.nextFloat();
}

sf::Color RandomMixColorGenerator::nextColor()
{
    if(colors_.size() != 3) return sf::Color();

    int randomIndex = rng_.nextByte() % 3;

    float mixRatio1 = (randomIndex == 0) ? rng_.nextFloat() * greyControl_ : rng_.nextFloat();
    float mixRatio2 = (randomIndex == 1) ? rng_.nextFloat() * greyControl_ : rng_.nextFloat();
    float mixRatio3 = (randomIndex == 2) ? rng_.nextFloat() * greyControl_ : rng_.nextFloat();

    float sum = mixRatio1 + mixRatio2 + mixRatio3;

    mixRatio1 /= sum;
    mixRatio2 /= sum;
    mixRatio3 /= sum;

    return sf::Color(
                static_cast<unsigned char>(mixRatio1 * colors_[0].r + mixRatio2 * colors_[1].r + mixRatio3 * colors_[2].r),
                static_cast<unsigned char>(mixRatio1 * colors_[0].g + mixRatio2 * colors_[1].g + mixRatio3 * colors_[2].g),
                static_cast<unsigned char>(mixRatio1 * colors_[0].b + mixRatio2 * colors_[1].b + mixRatio3 * colors_[2].b));
}

GoldenRatioRainbowColorGenerator::GoldenRatioRainbowColorGenerator(math::Random::Seed seed, uint32_t limit) : ColorGenerator(seed, limit)
{
    saturation_ = 0.1 + rng_.nextFloat() * 0.6;
    luminance_ = 0.4 + rng_.nextFloat() * 0.4;
    currentHue_ = rng_.nextFloat();
}

sf::Color GoldenRatioRainbowColorGenerator::nextColor()
{
    constexpr float goldenRatioConjugate = 0.618033988749895f;
    sf::HSL hslColor(currentHue_, saturation_, luminance_);
    currentHue_ += goldenRatioConjugate;
    currentHue_ = math::modulo(currentHue_, 1.0f);
    return hslColor();
}

GoldenRatioGradientColorGenerator::GoldenRatioGradientColorGenerator(math::Random::Seed seed, uint32_t limit) : ColorGenerator(seed, limit)
{
    saturation_ = 0.1 + rng_.nextFloat() * 0.6;
    luminance_ = 0.4 + rng_.nextFloat() * 0.4;
    currentHue_ = rng_.nextFloat();
    sf::Color mixColor1[] =
    {
        sf::Color(255, 0, 33),
        sf::Color(255, 100, 33),
        sf::Color(128, 0, 33),
    };

    sf::Color mixColor2[] =
    {
        sf::Color(255, 255, 0),
        sf::Color(255, 255, 100),
        sf::Color(255, 128, 0),
    };

    sf::Color mixColor3[] =
    {
        sf::Color(0, 100, 255),
        sf::Color(100, 150, 255),
        sf::Color(0, 80, 128),
    };
    int version = rng_.next(2);
    colors_ = std::vector<sf::Color>({mixColor1[version], mixColor2[version], mixColor3[version]});
}

sf::Color GoldenRatioGradientColorGenerator::nextColor()
{
    constexpr float goldenRatioConjugate = 0.618033988749895f;
    int colorCount = colors_.size();
    int leftIndex = static_cast<int>(currentHue_ * colorCount);

    float cellRange = 1.0f / colorCount;
    float alpha = (currentHue_ - leftIndex * cellRange) / cellRange;

    sf::Color leftColor = colors_[leftIndex];
    sf::Color rightColor = colors_[(leftIndex + 1) % colorCount];

    sf::Color color(
                static_cast<unsigned char>(leftColor.r * (1 - alpha) + rightColor.r * (alpha)),
                static_cast<unsigned char>(leftColor.g * (1 - alpha) + rightColor.g * (alpha)),
                static_cast<unsigned char>(leftColor.b * (1 - alpha) + rightColor.b * (alpha)));

    currentHue_ += goldenRatioConjugate;
    currentHue_ = math::modulo(currentHue_, 1.0f);
    return color;
}
namespace
{
sf::Color blendColors(const sf::Color& firstColor, const sf::Color& secondColor, float interpolation)
{
    assert(interpolation >= 0.f && interpolation <= 1.f);

    float firstPart = 1.f - interpolation;

    return sf::Color(
        static_cast<sf::Uint8>(firstPart * firstColor.r + interpolation * secondColor.r),
        static_cast<sf::Uint8>(firstPart * firstColor.g + interpolation * secondColor.g),
        static_cast<sf::Uint8>(firstPart * firstColor.b + interpolation * secondColor.b),
        static_cast<sf::Uint8>(firstPart * firstColor.a + interpolation * secondColor.a));
}
}

sf::Color GradientColorGenerator::nextColor()
{
    float position = float(index_) / float(limit_);

    assert(mColors.count(0.f) && mColors.count(1.f));

    // Find first entry >= position, return color if entry == position
    auto nextColor = mColors.lower_bound(position);
    if (nextColor->first == position)
        return nextColor->second;

    // Find first entry < position
    auto prevColor = std::prev(nextColor);

    // Interpolate color between 2 entries
    float interpolation = (position - prevColor->first) / (nextColor->first - prevColor->first);
    return blendColors(prevColor->second, nextColor->second, interpolation);
}

}
}

