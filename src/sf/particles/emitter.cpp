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

#include "emitter.h"
#include "particle.h"
#include "math/random.h"
#include "math/polar_vector.h"
#include "graphic/chart_colors.h"

namespace sf
{

Universal::Universal()
    : mEmissionRate(1.f)
    , mEmissionDifference(0.f)
    , mParticleLifetime(sf::seconds(1.f))
    , mParticlePosition(sf::Vector2f(0.f, 0.f))
    , mParticleVelocity(sf::Vector2f(0.f, 0.f))
    , mParticleRotation(0.f)
    , mParticleRotationSpeed(0.f)
    , mParticleScale(sf::Vector2f(1.f, 1.f))
    , mParticleColor(sf::Color::White)
    , mParticleTextureIndex(0u)
{
}

void Universal::operator() (EmissionInterface& system, sf::Time dt)
{
    const unsigned int nbParticles = computeParticleCount(dt);

    for (unsigned int i = 0; i < nbParticles; ++i)
    {
        // Create particle and specify parameters
        Particle particle( mParticleLifetime() );
        particle.position = mParticlePosition();
        particle.velocity = mParticleVelocity();
        particle.rotation = mParticleRotation();
        particle.rotationSpeed = mParticleRotationSpeed();
        particle.scale = mParticleScale();
        particle.color = mParticleColor();
        particle.textureIndex = mParticleTextureIndex();
        //
        system.emitParticle(particle);
    }
}

void Universal::setEmissionRate(float particlesPerSecond)
{
    mEmissionRate = particlesPerSecond;
}

void Universal::setParticleLifetime(beblob::math::Distribution<sf::Time> particleLifetime)
{
    mParticleLifetime.swap(particleLifetime);
}

void Universal::setParticlePosition(beblob::math::Distribution<sf::Vector2f> particlePosition)
{
    mParticlePosition.swap(particlePosition);
}

void Universal::setParticleVelocity(beblob::math::Distribution<sf::Vector2f> particleVelocity)
{
    mParticleVelocity.swap(particleVelocity);
}

void Universal::setParticleRotation(beblob::math::Distribution<float> particleRotation)
{
    mParticleRotation.swap(particleRotation);
}

void Universal::setParticleRotationSpeed(beblob::math::Distribution<float> particleRotationSpeed)
{
    mParticleRotationSpeed.swap(particleRotationSpeed);
}

void Universal::setParticleScale(beblob::math::Distribution<sf::Vector2f> particleScale)
{
    mParticleScale.swap(particleScale);
}

void Universal::setParticleColor(beblob::math::Distribution<sf::Color> particleColor)
{
    mParticleColor.swap(particleColor);
}

void Universal::setParticleTextureIndex(beblob::math::Distribution<unsigned int> particleTextureIndex)
{
    mParticleTextureIndex.swap(particleTextureIndex);
}

unsigned int Universal::computeParticleCount(sf::Time dt)
{
    // We want to fulfill the desired particle rate as exact as possible. Since the amount of emitted particles per frame is
    // integral, we have to emit sometimes more and sometimes less. mParticleDifference takes care of the deviation each frame.
    float particleAmount = mEmissionRate * dt.asSeconds() + mEmissionDifference;
    unsigned int nbParticles = static_cast<unsigned int>(particleAmount);

    // Compute difference for next frame, return current amount
    mEmissionDifference = particleAmount - nbParticles;
    return nbParticles;
}

void Firework::operator() (EmissionInterface& system, sf::Time dt)
{
    const sf::Time tailInterval = explosionDuration / tailsPerExplosion;

    // Accumulate passed time. If enough time has passed (tailInterval), emit a new tail and decrease accumulator.
    mAccumulatedTime += dt;
    while (mAccumulatedTime - tailInterval > sf::Time::Zero)
    {
        emitTail(system);
        mAccumulatedTime -= tailInterval;
    }
}

void Firework::emitTail(EmissionInterface& system)
{
    // Create initial direction with random vector length and angle
    static beblob::math::Random r;
    beblob::math::PolarVector2f velocity(r.range(30.f, 70.f), r.range(0.f, 360.f));

    static beblob::procedural::ChartColors<beblob::procedural::UniformColorGenerator> p(r.next(384));

    // Create particle at position of explosion, with emitter-specific color and at 80% initial scale
    Particle particle(tailDuration);
    particle.position = mPosition;
    //sf::Color Colors[] = {sf::Color(100, 100, 100),
    //                      sf::Color(150, 150, 150),
    //                      sf::Color(200, 200, 200)};
    //particle.color = Colors[r.next(2)];
    //particle.color = p.next();
    if(mColor == sf::Color::White)
        particle.color = p.next();
    else
        particle.color = mColor;
    particle.scale *= 0.8f;

    // A tail contains 25 particles with different speeds and scales.
    // The largest particles move fastest, leading to a comet-like tail effect.
    for (sf::Int64 i = 0; i < 25; ++i)
    {
        // Decrease scale continuously
        particle.scale *= 0.95f;

        // Decrease speed continuously
        velocity.r *= 0.96f;
        particle.velocity = velocity;

        // Add adapted particle to particle system
        system.emitParticle(particle);
    }
}
}
