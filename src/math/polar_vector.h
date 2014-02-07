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

#ifndef BEBLOB_POLAR_VECTOR_H
#define BEBLOB_POLAR_VECTOR_H

#include <SFML/System/Vector2.hpp>

namespace beblob
{
namespace math
{

template <typename T>
struct PolarVector2
{
    T	r;				///< Radius
    T	phi;			///< Angle in degrees

    PolarVector2();

    PolarVector2(T radius, T angle);

    PolarVector2(const sf::Vector2<T>& vector);

    operator sf::Vector2<T> () const;
};

typedef PolarVector2<float> PolarVector2f;

template <typename T>
T length(const PolarVector2<T>& vector);

template <typename T>
T polarAngle(const PolarVector2<T>& vector);

template <typename T>
PolarVector2<T>::PolarVector2()
: r()
, phi()
{
}

template <typename T>
PolarVector2<T>::PolarVector2(T radius, T angle)
: r(radius)
, phi(angle)
{
}

template <typename T>
PolarVector2<T>::PolarVector2(const sf::Vector2<T>& vector)
: r(length(vector))
, phi(vector == sf::Vector2<T>() ? 0.f : polarAngle(vector))
{
}

template <typename T>
PolarVector2<T>::operator sf::Vector2<T>() const
{
    return sf::Vector2<T>(
        r * std::cos(phi),
        r * std::sin(phi));
}

template <typename T>
T length(const PolarVector2<T>& vector)
{
    return vector.r;
}

template <typename T>
T polarAngle(const PolarVector2<T>& vector)
{
    return vector.phi;
}

}
}

#endif // BEBLOB_POLAR_VECTOR_H
