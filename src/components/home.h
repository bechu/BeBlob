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

#ifndef BEBLOB_GRAPHIC_HOME_H
#define BEBLOB_GRAPHIC_HOME_H

#include <SFML/Graphics.hpp>

#include "ecs/component.h"
#include "components/land_physic.h"
#include <sf/curve.h>

namespace beblob
{
namespace component
{

////////////////////////////////////////////////////////////
/// \brief home graphic instance
////////////////////////////////////////////////////////////
class Home
        : public ecs::Component<component::Home>
        , public sf::Drawable
        , public sf::Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief compute the shape of home
    ///
    /// \param land is physic description
    /// \param color team color
    ////////////////////////////////////////////////////////////
    void compute(component::LandSolid& land, const sf::Color& color);
    ////////////////////////////////////////////////////////////
    /// \brief draw home
    ///
    /// \param target where to draw
    /// \param states current transformation for this item
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    /// this is the collection of points/Colors used to draw a home
    sf::VertexArray shape_;
    /// outline of the house
    sf::Curve outline_;
};

}
}

#endif // BEBLOB_GRAPHIC_HOME_H
