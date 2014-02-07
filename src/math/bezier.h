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

#ifndef BEBLOB_GRAPHIC_BEZIER_H
#define BEBLOB_GRAPHIC_BEZIER_H

#include <SFML/System/Vector2.hpp>
#include <vector>

namespace beblob
{
namespace math
{

class Bezier
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief compute the raw points for a quadratic bezier curve
    ///
    /// \param segments the number of points desired
    /// \param p0 the initial point
    /// \param p1 the first control point
    /// \param p2 the second control point
    /// \param p3 the final point
    ///
    /// \see http://en.wikipedia.org/wiki/B%C3%A9zier_curve
    ////////////////////////////////////////////////////////////
    static std::vector<sf::Vector2f> compute(unsigned int segments,
                       const sf::Vector2f& p0,
                       const sf::Vector2f& p1,
                       const sf::Vector2f& p2,
                       const sf::Vector2f& p3);
private:
    ////////////////////////////////////////////////////////////
    /// \brief calculate a quadratic bezier point
    ///
    /// \param t where in the curve (0-1)
    /// \param p0 the initial point
    /// \param p1 the first control point
    /// \param p2 the second control point
    /// \param p3 the final point
    ///
    /// \see Bezier::compute
    ////////////////////////////////////////////////////////////
    static sf::Vector2f point(float t,
                       const sf::Vector2f& p0,
                       const sf::Vector2f& p1,
                       const sf::Vector2f& p2,
                       const sf::Vector2f& p3);
};

}
}

#endif // BEBLOB_GRAPHIC_BEZIER_H
