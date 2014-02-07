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

#ifndef BEBLOB_GRAPHIC_EMITTER_H
#define BEBLOB_GRAPHIC_EMITTER_H

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include "math/distribution.h"

namespace sf
{

class Particle;

struct EmissionInterface
{
    virtual ~EmissionInterface() {}
    virtual void emitParticle(const Particle& particle) = 0;
};

template <typename Emitter>
std::function<void(EmissionInterface&, sf::Time)> refEmitter(Emitter& referenced)
{
    return [&referenced] (EmissionInterface& system, sf::Time dt)
    {
            return referenced(system, dt);
};
}

class Universal
{
    // ---------------------------------------------------------------------------------------------------------------------------
    // Public member functions
public:
    Universal();
    void operator() (EmissionInterface& system, sf::Time dt);
    void setEmissionRate(float particlesPerSecond);
    void setParticleLifetime(beblob::math::Distribution<sf::Time> particleLifetime);
    void setParticlePosition(beblob::math::Distribution<sf::Vector2f> particlePosition);
    void setParticleVelocity(beblob::math::Distribution<sf::Vector2f> particleVelocity);
    void setParticleRotation(beblob::math::Distribution<float> particleRotation);
    //
    void setParticleRotationSpeed(beblob::math::Distribution<float> particleRotationSpeed);
    void setParticleScale(beblob::math::Distribution<sf::Vector2f> particleScale);
    void setParticleColor(beblob::math::Distribution<sf::Color> particleColor);
    void setParticleTextureIndex(beblob::math::Distribution<unsigned int> particleTextureIndex);
private:
    unsigned int	computeParticleCount(sf::Time dt);
private:
    float mEmissionRate;
    float mEmissionDifference;

    beblob::math::Distribution<sf::Time>		mParticleLifetime;
    beblob::math::Distribution<sf::Vector2f>	mParticlePosition;
    beblob::math::Distribution<sf::Vector2f>	mParticleVelocity;
    beblob::math::Distribution<float>			mParticleRotation;
    beblob::math::Distribution<float>			mParticleRotationSpeed;
    beblob::math::Distribution<sf::Vector2f>	mParticleScale;
    beblob::math::Distribution<sf::Color>		mParticleColor;
    beblob::math::Distribution<unsigned int>	mParticleTextureIndex;
};

class Firework
{
public:
    explicit Firework(sf::Vector2f position, const sf::Color& color=sf::Color::White)
        : mAccumulatedTime(sf::Time::Zero)
        , mPosition(position)
        , mColor(color)
    {
    }

    void operator() (EmissionInterface& system, sf::Time dt);

private:
    void emitTail(EmissionInterface& system);

private:
    sf::Time		mAccumulatedTime;
    sf::Vector2f	mPosition;
    sf::Color		mColor;
    //const sf::Time	explosionInterval	= sf::seconds(1.f);
    const sf::Time	explosionDuration	= sf::seconds(0.2f);
    const sf::Time	tailDuration		= sf::seconds(1.5f);
    const sf::Int64	tailsPerExplosion	= 10;
};

}

#endif // BEBLOB_GRAPHIC_EMITTER_H
