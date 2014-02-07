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

#include "surface.h"
#include "math/random.h"

namespace beblob
{
namespace physic
{

Surface::Surface()
{
    init(sf::Vector2i());
}

Surface::Surface(const sf::Vector2i& location)
{
    init(location);
}

void Surface::init(const sf::Vector2i& location)
{
    Max = Min = location;
}

uint32_t Surface::width() const
{
    return Max.x - Min.x;
}

uint32_t Surface::height() const
{
    return Max.y - Min.y;
}

Surface& Surface::operator = (const Surface& s)
{
    Min = s.Min;
    Max = s.Max;
    return *this;
}

bool Surface::operator==(const Surface& s) const
{
    if(Min == s.Min && Max == s.Max)
        return true;
    return false;
}

bool Surface::operator!=(const Surface& s) const
{
    if(Min != s.Min || Max != s.Max)
        return true;
    return false;
}

bool Surface::contains(const sf::Vector2i& point) const
{
    return (point.x >= Min.x && point.x <= Max.x
            && point.y >= Min.y && point.y <= Max.y);
}

bool Surface::contains(const Surface& other) const
{
    return (contains(other.Min) && contains(other.Max));
}

bool Surface::isAligned(const Surface& other) const
{
    if(Min.x == other.Min.x && Max.x == other.Max.x)
    {
        if(Max.y == other.Min.y-1)
            return true;
        if(Min.y == other.Max.y+1)
            return true;
    }
    if(Min.y == other.Min.y && Max.y == other.Max.y)
    {
        if(Max.x == other.Min.x-1)
            return true;
        if(Min.x == other.Max.x+1)
            return true;
    }
    return false;
}

void Surface::merge(const Surface& other)
{
    Max.x = std::max(other.Max.x, Max.x);
    Max.y = std::max(other.Max.y, Max.y);
    Min.x = std::min(other.Min.x, Min.x);
    Min.y = std::min(other.Min.y, Min.y);
}

void Surface::publish(component::LandSolid& land)
{
    math::Random rng;

    int32_t wx = 1 + Max.x - Min.x;
    int32_t wy = 1 + Max.y - Min.y;

    land.points.clear();
    land.width = wx * component::LandSolid::kSize;
    land.height = wy * component::LandSolid::kSize;
    land.x = Min.x * component::LandSolid::kSize;
    land.y = Min.y * component::LandSolid::kSize;

    land.points.push_back(sf::Vector2f(land.x, land.y));

    land.points.push_back(sf::Vector2f(land.x+rng.next(20), land.y+land.height/2));

    land.points.push_back(sf::Vector2f(land.x, land.y+land.height));

    land.points.push_back(sf::Vector2f(land.x+land.width, land.y+land.height));

    land.points.push_back(sf::Vector2f(land.x+land.width-rng.next(20), land.y+land.height/2));

    land.points.push_back(sf::Vector2f(land.x+land.width, land.y));
}

void Surface::publish_home(component::LandSolid& land)
{
    math::Random rng;

    int32_t wx = 1 + Max.x - Min.x;
    int32_t wy = 1 + Max.y - Min.y;

    land.points.clear();
    land.width = wx * component::LandSolid::kSize;
    land.height = wy * component::LandSolid::kSize;
    land.x = Min.x * component::LandSolid::kSize;
    land.y = Min.y * component::LandSolid::kSize;

    land.points.push_back(sf::Vector2f(land.x+land.width/2, land.y));
    land.points.push_back(sf::Vector2f(land.x, land.y+20));

    //land.points.push_back(sf::Vector2f(land.x+rng.next(20), land.y+land.height/2));

    land.points.push_back(sf::Vector2f(land.x, land.y+land.height));

    land.points.push_back(sf::Vector2f(land.x+land.width, land.y+land.height));

    //land.points.push_back(sf::Vector2f(land.x+land.width-rng.next(20), land.y+land.height/2));

    land.points.push_back(sf::Vector2f(land.x+land.width, land.y+20));
}

std::vector<Surface> Surface::remove(const sf::Vector2i& location)
{
    std::vector<Surface> ss;
    Surfaces sur;
    for(int i=Min.x;i<=Max.x;++i)
    {
        for(int j=Min.y;j<=Max.y;++j)
        {
            if(i == location.x && j == location.y) continue ;
            sur.append(Surface(sf::Vector2i(i, j)));
        }
    }
    for(auto s : sur)
        ss.push_back(s);
    return ss;
}

bool Surfaces::merge(const Surface& surface)
{
    for(Surfaces::iterator it=begin();it!=end();++it)
    {
        if(surface.isAligned(*it))
        {
            (*it).merge(surface);
            for(Surfaces::iterator it2=begin();it2!=end();++it2)
            {
                if((*it).isAligned(*it2))
                {
                    (*it).merge(*it2);
                    erase(it2);
                    return true;
                }
            }
            return true;
        }
    }
    push_back(surface);
    return false;
}

void Surfaces::append(const Surface& surface)
{
    merge(surface);
}

}
}
