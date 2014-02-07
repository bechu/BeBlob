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

#ifndef BEBLOB_GRAPHIC_PARTICULE_H
#define BEBLOB_GRAPHIC_PARTICULE_H

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
{

class Particle
{
public:
    explicit Particle(sf::Time lifetime);

    sf::Vector2f position;			///< Current position.
    sf::Vector2f velocity;			///< Velocity (change in position per second).
    float rotation;			///< Current rotation angle.
    float rotationSpeed;		///< Angular velocity (change in rotation per second).
    sf::Vector2f scale;				///< Scale, where (1,1) represents the original size.
    sf::Color color;				///< %Particle color.
    unsigned int textureIndex;		///< Index of the used texture rect, returned by ParticleSystem::addTextureRect()

private:
    sf::Time					passedLifetime;		// Time passed since emitted.
    sf::Time					totalLifetime;		// Total time to live.
    friend class ParticleSystem;
    friend sf::Time getElapsedLifetime(const Particle& particle);
    friend sf::Time getTotalLifetime(const Particle& particle);
};

sf::Time 		getElapsedLifetime(const Particle& particle);

sf::Time 		getTotalLifetime(const Particle& particle);

sf::Time 		getRemainingLifetime(const Particle& particle);

float getElapsedRatio(const Particle& particle);

float getRemainingRatio(const Particle& particle);

}

#endif // BEBLOB_GRAPHIC_PARTICULE_H
