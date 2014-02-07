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

#ifndef BEBLOB_PHYSICS_ENV_H
#define BEBLOB_PHYSICS_ENV_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace beblob
{
namespace component
{

class Env
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Env constructor
    ////////////////////////////////////////////////////////////
    Env();

    ////////////////////////////////////////////////////////////
    /// \brief test collision with environnemnt
    ///
    /// \param pos vector to test, but update in same time too
    ///
    /// \return true if collide else false
    ////////////////////////////////////////////////////////////
    bool collide(sf::Vector2f& pos) const;
private:
    /// contains the outline rect dimension of the world
    sf::FloatRect rect_;
};

}
}

#endif // BEBLOB_PHYSICS_ENV_H
