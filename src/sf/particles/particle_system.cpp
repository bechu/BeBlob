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


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cassert>


#include "particle_system.h"

namespace sf
{

namespace
{

    // Erases emitter/affector at itr from ctr, if its time has expired. itr will point to the next element.
    template <class Container>
    void incrementCheckExpiry(Container& ctr, typename Container::iterator& itr, sf::Time dt)
    {
        // itr->second is the remaining time of the emitter/affector.
        // Time::Zero means infinite time (no removal).
        if (itr->timeUntilRemoval != sf::Time::Zero && (itr->timeUntilRemoval -= dt) <= sf::Time::Zero)
            itr = ctr.erase(itr);
        else
            ++itr;
    }

    sf::IntRect getFullRect(const sf::Texture& texture)
    {
        return sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
    }

}



ParticleSystem::ParticleSystem()
: mParticles()
, mAffectors()
, mEmitters()
, mTexture(nullptr)
, mTextureRects()
, mVertices(sf::Quads)
, mNeedsVertexUpdate(true)
, mQuads()
, mNeedsQuadUpdate(true)
{
}

void ParticleSystem::setTexture(const sf::Texture& texture)
{
    mTexture = &texture;
    mNeedsQuadUpdate = true;
}

unsigned int ParticleSystem::addTextureRect(const sf::IntRect& textureRect)
{
    mTextureRects.push_back(textureRect);
    mNeedsQuadUpdate = true;

    return mTextureRects.size() - 1;
}

void ParticleSystem::swap(ParticleSystem& other)
{
    // Use ADL
    using std::swap;

    swap(mParticles,			other.mParticles);
    swap(mAffectors,			other.mAffectors);
    swap(mEmitters,				other.mEmitters);
    swap(mTexture,				other.mTexture);
    swap(mTextureRects,			other.mTextureRects);
    swap(mVertices,				other.mVertices);
    swap(mNeedsVertexUpdate,	other.mNeedsVertexUpdate);
    swap(mQuads,				other.mQuads);
    swap(mNeedsQuadUpdate,		other.mNeedsQuadUpdate);
}

void ParticleSystem::addAffector(std::function<void(Particle&, sf::Time)> affector)
{
    addAffector(std::move(affector), sf::Time::Zero);
}

void ParticleSystem::addAffector(std::function<void(Particle&, sf::Time)> affector, sf::Time timeUntilRemoval)
{
    mAffectors.push_back( Affector(std::move(affector), timeUntilRemoval) );
    //@@@mAffectors.back().tracker = detail::makeIdConnectionImpl(mAffectors, mAffectors.back().id);

    //@@@mAffectors.back().tracker;
}

void ParticleSystem::clearAffectors()
{
    mAffectors.clear();
}

void ParticleSystem::addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter)
{
    addEmitter(emitter, sf::Time::Zero);
}

void ParticleSystem::addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter, sf::Time timeUntilRemoval)
{
    mEmitters.push_back( Emitter(std::move(emitter), timeUntilRemoval) );
    //mEmitters.back().tracker = detail::makeIdConnectionImpl(mEmitters, mEmitters.back().id);

    //mEmitters.back().tracker;
}

void ParticleSystem::clearEmitters()
{
    mEmitters.clear();
}

void ParticleSystem::update(double t)
{
    sf::Time dt = sf::milliseconds(t);
    // Invalidate stored vertices
    mNeedsVertexUpdate = true;

    // Emit new particles and remove expiring emitters
    for (Emitters::iterator itr = mEmitters.begin(); itr != mEmitters.end(); )
    {
        itr->function(*this, dt);
        incrementCheckExpiry(mEmitters, itr, dt);
    }

    // Affect existing particles
    Particles::iterator writer = mParticles.begin();
    for (Particles::iterator reader = mParticles.begin(); reader != mParticles.end(); ++reader)
    {
        // Apply movement and decrease lifetime
        updateParticle(*reader, dt);

        // If current particle is not dead
        if (reader->passedLifetime < reader->totalLifetime)
        {
            // Only apply affectors to living particles
            for(auto &affectorPair : mAffectors)
                affectorPair.function(*reader, dt);

            // Go ahead
            *writer++ = *reader;
        }
    }

    // Remove particles dying this frame
    mParticles.erase(writer, mParticles.end());

    // Remove affectors expiring this frame
    for (Affectors::iterator itr = mAffectors.begin(); itr != mAffectors.end(); )
    {
        incrementCheckExpiry(mAffectors, itr, dt);
    }
}

void ParticleSystem::clearParticles()
{
    mParticles.clear();
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Check cached vertices
    if (mNeedsVertexUpdate)
    {
        computeVertices();
        mNeedsVertexUpdate = false;
    }

    // Check cached rectangles
    if (mNeedsQuadUpdate)
    {
        computeQuads();
        mNeedsQuadUpdate = false;
    }

    // Draw the vertex array with our texture
    states.texture = mTexture;
    target.draw(mVertices, states);
}

void ParticleSystem::emitParticle(const Particle& particle)
{
    mParticles.push_back(particle);
}

void ParticleSystem::updateParticle(Particle& particle, sf::Time dt)
{
    particle.passedLifetime += dt;

    particle.position += dt.asSeconds() * particle.velocity;
    particle.rotation += dt.asSeconds() * particle.rotationSpeed;
}

void ParticleSystem::computeVertices() const
{
    // Clear vertex array (keeps memory allocated)
    mVertices.clear();

    // Fill vertex array
    for(auto p : mParticles)
    {
        sf::Transform transform;
        transform.translate(p.position);
        transform.rotate(p.rotation);
        transform.scale(p.scale);

        // Ensure valid index -- if this fails, you have not called addTextureRect() enough times, or p.textureIndex is simply wrong
        assert(p.textureIndex == 0 || p.textureIndex < mTextureRects.size());

        const auto& quad = mQuads[p.textureIndex];
        for (unsigned int i = 0; i < 4; ++i)
        {
            sf::Vertex vertex;
            vertex.position = transform.transformPoint(quad[i].position);
            vertex.texCoords = quad[i].texCoords;
            vertex.color = p.color;

            mVertices.append(vertex);
        }
    }
}

void ParticleSystem::computeQuads() const
{
    // Ensure setTexture() has been called
    assert(mTexture);

    // No texture rects: Use full texture, cache single rectangle
    if (mTextureRects.empty())
    {
        mQuads.resize(1);
        computeQuad(mQuads[0], getFullRect(*mTexture));
    }

    // Specified texture rects: Cache every one
    else
    {
        mQuads.resize(mTextureRects.size());
        for (std::size_t i = 0; i < mTextureRects.size(); ++i)
            computeQuad(mQuads[i], mTextureRects[i]);
    }
}

void ParticleSystem::computeQuad(Quad& quad, const sf::IntRect& textureRect) const
{
    sf::FloatRect rect(textureRect);

    quad[0].texCoords = sf::Vector2f(rect.left,              rect.top);
    quad[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
    quad[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
    quad[3].texCoords = sf::Vector2f(rect.left,              rect.top + rect.height);

    quad[0].position = sf::Vector2f(-rect.width, -rect.height) / 2.f;
    quad[1].position = sf::Vector2f( rect.width, -rect.height) / 2.f;
    quad[2].position = sf::Vector2f( rect.width,  rect.height) / 2.f;
    quad[3].position = sf::Vector2f(-rect.width,  rect.height) / 2.f;
}

}
