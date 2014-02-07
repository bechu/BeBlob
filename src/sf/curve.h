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

#ifndef BEBLOB_GRAPHIC_CURVE_H
#define BEBLOB_GRAPHIC_CURVE_H

#include <SFML/Graphics.hpp>

namespace sf
{

////////////////////////////////////////////////////////////
/// \brief a SFML curve
////////////////////////////////////////////////////////////
class Curve
        : public sf::Drawable
        , public sf::Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Curve();

    ////////////////////////////////////////////////////////////
    /// \brief add a new point
    ///
    /// \param thickness the width of this point
    /// \param color ...
    ////////////////////////////////////////////////////////////
    void add(const sf::Vector2f& point,
             uint32_t thickness=2,
             const sf::Color& color=sf::Color(255, 255, 0));

    ////////////////////////////////////////////////////////////
    /// \brief clear the path
    ////////////////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////////////////
    /// \brief return the size
    ///
    /// \param size_t path size
    ////////////////////////////////////////////////////////////
    size_t size() const;

    ////////////////////////////////////////////////////////////
    /// \brief draw the path
    ///
    /// \param sfml target render
    /// \param states
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    /// list of the vertices
    sf::VertexArray vertices_;
    /// the previous point
    sf::Vector2f previous_;
    /// past thickness
    unsigned int pwidth_;
    /// past color
    sf::Color color_;
};

inline size_t Curve::size() const
{
    return vertices_.getVertexCount();
}

}

#endif // BEBLOB_GRAPHIC_CURVE_H
