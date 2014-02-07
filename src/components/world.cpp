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

#include <cstring>
#include <sstream>

#include "world.h"
#include "math/perlin_noise.h"
#include "math/range.h"
#include "math/vector.h"
#include "components/land_physic.h"

namespace beblob
{
namespace component
{

World::Cell component::World::dummy_;

bool component::World::Cell::isCollisionable() const
{
    return (type == kWall || type == kHome || type == kUnbreakable);
}

World::World()
{
    dummy_.type = Cell::kUnbreakable;
}

void World::create(math::Random::Seed seed, uint32_t player_count)
{
    rng_.setSeed(seed);
    generate(seed);
    homes_.clear();
    while(homes_.size() != player_count)
    {
        homes_.clear();
        uint32_t p = 0;
        while(p < player_count)
        {
            sf::Vector2i home;
            home.x = rng_.next(kWidth-2)+1;
            home.y = rng_.next(kHeight-2)+1;

            if(farFromOthers(home, kWidth) == false)
            {
                break;
            }

            homes_.push_back(home);
            p++;
        }
    }
    for(auto& home : homes_)
        cleanHome(home);

}

void component::World::setType(const sf::Vector2i& c, component::World::Cell::Type type)
{
    getCell(c).type = type;
    avert_observers(c);
}

World::Cell& component::World::getCell(const sf::Vector2i& c)
{
    if(c.x >= kWidth || c.x < 0 || c.y >= kHeight || c.y < 0)
        return dummy_;
    return cells_[ c.y * kWidth + c.x ];
}

World::Cell component::World::getCell(const sf::Vector2i& c) const
{
    if(c.x >= kWidth || c.x < 0 || c.y >= kHeight || c.y < 0)
        return dummy_;
    return cells_[ c.y * kWidth + c.x ];
}

void component::World::setResistance(const sf::Vector2i& c, int resistance)
{
    getCell(c).Resistance = resistance;
    avert_observers(c);
}

void component::World::setFood(const sf::Vector2i& c, int food)
{
    getCell(c).Food = food;
    avert_observers(c);
}

void component::World::setBuild(const sf::Vector2i& c, int build)
{
    getCell(c).Build = build;
    avert_observers(c);
}

bool World::farFromOthers(const sf::Vector2i& home, int32_t count)
{
    for(auto h : homes_)
    {
        if(math::distance(home, h) < count)
            return false;
    }
    return true;
}

void World::cleanHome(const sf::Vector2i& home)
{
    using namespace component;

    for(int i=-1;i<=1;i++)
    {
        for(int j=-1;j<=1;j++)
        {
            sf::Vector2i loc(home.x+i, home.y+j);
            if(loc.x >= 0 && loc.x < kWidth
                    && loc.y >= 0 && loc.y < kHeight)
                (*this).setType(loc, World::Cell::kNothing);
        }
    }

    (*this).setType(home, World::Cell::kHome);
    getCell(home).Resistance = Cell::kDefaultResistanceForHome;
}

void World::generate(math::Random::Seed seed)
{
    cells_.clear();
    cells_.resize(kWidth * kHeight);
    for(auto& cell : cells_)
    {
        cell.type = Cell::kNothing;
        cell.Resistance = Cell::kDefaultResistance;
        cell.Food = Cell::kDefaultFood;
        cell.Build = 0;
    }
    PerlinNoise p(seed);
    for(int i=0;i<kWidth;++i)
    {
        for(int j=0;j<kHeight;++j)
        {
            float u = p.noise(10 * float(i), 10 * float(j), 0.8);
            if( u > .58)
            {
                getCell(sf::Vector2i(i, j)).type = Cell::kFood;
                getCell(sf::Vector2i(i, j)).Food = Cell::kDefaultFood;
            }
            else if(u > .45)
            {
                getCell(sf::Vector2i(i, j)).type = Cell::kWall;
                getCell(sf::Vector2i(i, j)).Resistance = Cell::kDefaultResistance;
            }
        }
    }
}

sf::Vector2i World::screen2map(const sf::Vector2f& pos)
{
    if(pos.x < 0 || pos.y < 0) return sf::Vector2i(-1, -1);
    return sf::Vector2i(pos.x/LandSolid::kSize, pos.y/LandSolid::kSize);
}

sf::Vector2f World::map2screen(const sf::Vector2i& pos)
{
    if(pos.x < 0 || pos.y < 0) return sf::Vector2f(-1, -1);
    return sf::Vector2f(pos.x*component::LandSolid::kSize+(LandSolid::kSize/2),
                        pos.y*component::LandSolid::kSize+(LandSolid::kSize/2));
}

void World::avert_observers(const sf::Vector2i& c)
{
    for(auto& observer : observers_)
        observer->updated(c, (*this)(c));
}

void World::addObserver(World::Observer::Ptr observer)
{
    observers_.push_back(observer);
}

World& World::operator= (const World& rhs)
{
    cells_.resize(rhs.cells_.size());
    std::copy(rhs.cells_.begin(), rhs.cells_.end(), cells_.begin());
    homes_.resize(rhs.homes_.size());
    std::copy(rhs.homes_.begin(), rhs.homes_.end(), homes_.begin());
    return *this;
}

void World::set(const sf::Vector2i& c, const component::World::Cell& cell)
{
    getCell(c) = cell;
    avert_observers(c);
}

}
}
