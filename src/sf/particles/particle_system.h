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

#ifndef BEBLOB_GRAPHIC_PARTICULE_SYSTEM_H
#define BEBLOB_GRAPHIC_PARTICULE_SYSTEM_H

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <array>

#include "particle.h"
#include "emitter.h"
#include "affector.h"

#include "ecs/component.h"

namespace sf
{

class ParticleSystem :
        public sf::Drawable,
        private sf::NonCopyable,
        private EmissionInterface,
        public ecs::Component<ParticleSystem>
{
public:
    // Type to store affector or emitter + time until removal + id for removal
    template <typename Signature>
    struct Function
    {
        Function(std::function<Signature> function, sf::Time timeUntilRemoval)
            : function(std::move(function))
            , timeUntilRemoval(timeUntilRemoval)
            , id(nextId())
        {
        }

        static unsigned int nextId()
        {
            static unsigned int next = 0;
            return next++;
        }

        std::function<Signature>						function;
        sf::Time										timeUntilRemoval;
        unsigned int									id;
        //std::shared_ptr<detail::AbstractConnectionImpl> tracker;
    };

    // Vertex quads, used to cache texture rectangles
    typedef std::array<sf::Vertex, 4>					Quad;
    // Function typedefs
    typedef Function<void(Particle&, sf::Time)>			Affector;
    typedef Function<void(EmissionInterface&, sf::Time)>	Emitter;

    typedef std::vector<Particle>						Particles;
    typedef std::vector<Affector>						Affectors;
    typedef std::vector<Emitter>						Emitters;
    // ---------------------------------------------------------------------------------------------------------------------------
    // Public member functions
public:
    /// @brief Default constructor
    /// @details Requires a call to setTexture() and possibly addTextureRect() before the particle system can be used.
    ParticleSystem();

    void 	swap(ParticleSystem& other);

    void setTexture(const sf::Texture& texture);

    unsigned int	addTextureRect(const sf::IntRect& textureRect);

    void	 addAffector(std::function<void(Particle&, sf::Time)> affector);

    void	 addAffector(std::function<void(Particle&, sf::Time)> affector, sf::Time timeUntilRemoval);

    void clearAffectors();

    void addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter);

    void addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter, sf::Time timeUntilRemoval);

    void clearEmitters();

    void update(double t);

    void clearParticles();
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void emitParticle(const Particle& particle);
    void						updateParticle(Particle& particle, sf::Time dt);
    void						computeVertices() const;
    void						computeQuads() const;
    void						computeQuad(Quad& quad, const sf::IntRect& textureRect) const;

    Particles mParticles;
    Affectors mAffectors;
    Emitters mEmitters;

    const sf::Texture*			mTexture;
    std::vector<sf::IntRect>	mTextureRects;

    mutable sf::VertexArray		mVertices;
    mutable bool				mNeedsVertexUpdate;
    mutable std::vector<Quad> mQuads;
    mutable bool				mNeedsQuadUpdate;
};

}

#endif // BEBLOB_GRAPHIC_PARTICULE_SYSTEM_H
