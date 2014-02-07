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

#include "bezier.h"

namespace beblob
{
namespace math
{

std::vector<sf::Vector2f> Bezier::compute(unsigned int segments,
                       const sf::Vector2f& p0,
                       const sf::Vector2f& p1,
                       const sf::Vector2f& p2,
                       const sf::Vector2f& p3)
{
    std::vector<sf::Vector2f> points;
    for (unsigned int j = 0; j <= segments; j++)
    {
        float t = static_cast<float>(j) / static_cast<float>(segments);
        points.push_back(Bezier::point(t, p0, p1, p2, p3));
    }
    return points;
}

sf::Vector2f Bezier::point(float t,
                           const sf::Vector2f& p0,
                           const sf::Vector2f& p1,
                           const sf::Vector2f& p2,
                           const sf::Vector2f& p3)
{
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    sf::Vector2f p = uuu * p0;

    p += 3 * uu * t * p1;
    p += 3 * u * tt * p2;
    p += ttt * p3;
    return p;
}

}
}
