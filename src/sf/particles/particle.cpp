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

#include "particle.h"

namespace sf
{

Particle::Particle(sf::Time totalLifetime)
: position()
, velocity()
, rotation()
, rotationSpeed()
, scale(1.f, 1.f)
, color(255, 255, 255)
, textureIndex(0)
, passedLifetime(sf::Time::Zero)
, totalLifetime(totalLifetime)
{
}

sf::Time getElapsedLifetime(const Particle& particle)
{
    return particle.passedLifetime;
}

sf::Time getTotalLifetime(const Particle& particle)
{
    return particle.totalLifetime;
}

sf::Time getRemainingLifetime(const Particle& particle)
{
    return getTotalLifetime(particle) - getElapsedLifetime(particle);
}

float getElapsedRatio(const Particle& particle)
{
    return getElapsedLifetime(particle).asSeconds() / getTotalLifetime(particle).asSeconds();
}

float getRemainingRatio(const Particle& particle)
{
    return getRemainingLifetime(particle).asSeconds() / getTotalLifetime(particle).asSeconds();
}

}
