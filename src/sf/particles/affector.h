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

#ifndef BEBLOB_GRAPHIC_AFFECTOR_H
#define BEBLOB_GRAPHIC_AFFECTOR_H

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>

namespace sf
{

class Particle;

struct Affector
{
    // ##############################
    class Force
    {
    public:
        explicit Force(const sf::Vector2f& acceleration);
        void operator() (Particle& particle, sf::Time dt);
        void setAcceleration(const sf::Vector2f& acceleration);
        sf::Vector2f getAcceleration() const;
    private:
        sf::Vector2f acceleration_;
    };

    // ##############################
    class Torque
    {
        public:
            explicit Torque(float angularAcceleration);
            void operator() (Particle& particle, sf::Time dt);
            void setAngularAcceleration(float angularAcceleration);
            float getAngularAcceleration() const;
        private:
            float angularAcceleration_;
    };

    // ##############################
    class Scale
    {
        public:
            explicit Scale(const sf::Vector2f& scaleFactor);
            void operator() (Particle& particle, sf::Time dt);
            void setScaleFactor(const sf::Vector2f& scaleFactor);
            sf::Vector2f getScaleFactor() const;
        private:
            sf::Vector2f scaleFactor_;
    };

    // ##############################
    class Animation
    {
        public:
            typedef std::function<void(Particle&, float)> Func;
            explicit Animation(const Func& particleAnimation);
            void operator() (Particle& particle, sf::Time dt);
        private:
            Func animation_;
    };

    // ##############################
    class Firework
    {
        public:
        void operator() (Particle& particle, sf::Time dt);
        const float	 gravity = 30.f;
    };

};

}

#endif // BEBLOB_GRAPHIC_AFFECTOR_H
