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

#ifndef BEBLOB_MATH_VECTOR_H
#define BEBLOB_MATH_VECTOR_H

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace beblob
{
namespace math
{

////////////////////////////////////////////////////////////
/// \brief compute distance from two vector
///
/// \tparam type of vector (float, int)
/// \param from the first vector
/// \param to the second vector
///
/// \return the computed distance
////////////////////////////////////////////////////////////
template <typename T>
inline T distance(const sf::Vector2<T>& from, const sf::Vector2<T>& to)
{
    return std::sqrt(std::pow(to.x-from.x,2)+std::pow(to.y-from.y,2));
}

////////////////////////////////////////////////////////////
/// \brief compute the length of a vector
///
/// \tparam type of vector (float, int)
/// \param v the vector
///
/// \return the computed length
////////////////////////////////////////////////////////////
template <typename T>
inline static T length(const sf::Vector2<T>& v)
{
  return std::sqrt(v.x*v.x + v.y*v.y);
}

////////////////////////////////////////////////////////////
/// \brief normalize a vector
///
/// \tparam type of vector (float, int)
/// \param v the vector
///
/// \return a new normalized vector
////////////////////////////////////////////////////////////
template <typename T>
inline sf::Vector2<T> normalize(const sf::Vector2<T>& vec)
{
  float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
  return length ? sf::Vector2<T>(vec.x / length, vec.y / length) : vec;
}

////////////////////////////////////////////////////////////
/// \brief normalize a vector
///
/// \tparam type of vector (float, int)
/// \param v the vector
/// \param scaleFactor the scale factor
////////////////////////////////////////////////////////////
template <typename T>
void normalize(sf::Vector2<T>& v, T scaleFactor)
{
    scaleFactor = scaleFactor / std::sqrt(v.x*v.x + v.y*v.y);
    v.x *= scaleFactor;
    v.y *= scaleFactor;
}

////////////////////////////////////////////////////////////
/// \brief compute a squared distance from two vector
///
/// \tparam type of vector (float, int)
/// \param lhs first vector
/// \param rhs secon vector
///
/// \return the squared distance
////////////////////////////////////////////////////////////
template <typename T>
float distance_squared(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
{
    float t1 = lhs.x - rhs.x;
    float t2 = lhs.y - rhs.y;
    return t1*t1 + t2*t2;
}

////////////////////////////////////////////////////////////
/// \brief cross two vector
///
/// \tparam type of vector (float, int)
/// \param lhs first vector
/// \param rhs secon vector
///
/// \return the vector updated
////////////////////////////////////////////////////////////
template <typename T>
inline T cross(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
{
  return lhs.x * rhs.x + lhs.y * rhs.y;
}

////////////////////////////////////////////////////////////
/// \brief compute a distance between a line and a point
///
/// \tparam type of vector (float, int)
/// \param a first vector of the line
/// \param b secon vector of the line
/// \param point point
///
/// \return the computed minimal distance
////////////////////////////////////////////////////////////
template <typename T>
inline T distance_to_Line(const sf::Vector2<T>& a,
                              const sf::Vector2<T>& b,
                              const sf::Vector2<T>& point)
{
    float lengthAB = std::hypot(b.x - a.x, b.y - a.y) ;
    return std::fabs((point.x - a.x) * (b.y - a.y)
                     - (point.y - a.y) * (b.x - a.x)) / lengthAB;
}

}
}

#endif // BEBLOB_MATH_VECTOR_H
