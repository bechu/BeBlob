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

#include "affector.h"
#include "particle.h"

namespace sf
{

Affector::Force::Force(const sf::Vector2f& acceleration) : acceleration_(acceleration)
{
}
void Affector::Force::operator() (Particle& particle, sf::Time dt)
{
    particle.velocity += dt.asSeconds() * acceleration_;
}

void Affector::Force::setAcceleration(const sf::Vector2f& acceleration)
{
    acceleration_ = acceleration;
}

sf::Vector2f Affector::Force::getAcceleration() const
{
    return acceleration_;
}

// ***************************************************************************************************

Affector::Torque::Torque(float angularAcceleration) : angularAcceleration_(angularAcceleration)
{
}

void Affector::Torque::operator() (Particle& particle, sf::Time dt)
{
    particle.rotationSpeed += dt.asSeconds() * angularAcceleration_;
}

void Affector::Torque::setAngularAcceleration(float angularAcceleration)
{
    angularAcceleration_ = angularAcceleration;
}

float Affector::Torque::getAngularAcceleration() const
{
    return angularAcceleration_;
}

// ***************************************************************************************************
Affector::Scale::Scale(const sf::Vector2f& scaleFactor) : scaleFactor_(scaleFactor)
{
}

void Affector::Scale::operator() (Particle& particle, sf::Time dt)
{
    particle.scale += dt.asSeconds() * scaleFactor_;
}

void Affector::Scale::setScaleFactor(const sf::Vector2f& scaleFactor)
{
    scaleFactor_ = scaleFactor;
}

sf::Vector2f Affector::Scale::getScaleFactor() const
{
    return scaleFactor_;
}

// ***************************************************************************************************
Affector::Animation::Animation(const Affector::Animation::Func& particleAnimation) : animation_(particleAnimation)
{
}

void Affector::Animation::operator() (Particle& particle, sf::Time)
{
    animation_(particle, getElapsedRatio(particle));
}

// ***************************************************************************************************
void Affector::Firework::operator() (Particle& particle, sf::Time dt)
{
    // Apply gravity, where particles with greater scale are affected stronger (quadratic influence)
    particle.velocity += dt.asSeconds() * sf::Vector2f(0.f, gravity) * particle.scale.x * particle.scale.y;

    // Let particles continuously fade out (particles with smaller scale have already lower alpha value at beginning)
    particle.color.a = static_cast<sf::Uint8>(256 * getRemainingRatio(particle) * particle.scale.x);
}

}
