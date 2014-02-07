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

#ifndef BLOB_POINTMASS_H
#define BLOB_POINTMASS_H

#include <SFML/System/Vector2.hpp>
#include <memory>

namespace beblob
{
namespace physic
{

////////////////////////////////////////////////////////////
/// \brief point mass class
////////////////////////////////////////////////////////////
class PointMass
{
public:
    /// point mass shared pointer definition
    typedef std::shared_ptr<PointMass> Ptr;

    ////////////////////////////////////////////////////////////
    /// \brief Creation
    ////////////////////////////////////////////////////////////
    PointMass();

    ////////////////////////////////////////////////////////////
    /// \brief Creation
    ///
    /// \param xPos x position
    /// \param yPos y position
    /// \param aMass
    ////////////////////////////////////////////////////////////
    PointMass(float xPos, float yPos, float aMass);

    ////////////////////////////////////////////////////////////
    /// \brief Point mass copy creation
    ///
    /// \param that the origin class
    ////////////////////////////////////////////////////////////
    PointMass(const PointMass& that);

    ////////////////////////////////////////////////////////////
    /// \brief move the point mass
    ///
    /// \param dt diff time
    ////////////////////////////////////////////////////////////
    void move(float dt);

    ////////////////////////////////////////////////////////////
    /// \brief set the friction
    ///
    /// \param friction new friction value
    ////////////////////////////////////////////////////////////
    void setFriction(float friction);

    ////////////////////////////////////////////////////////////
    /// \brief set the force
    ///
    /// \param force the new force
    ////////////////////////////////////////////////////////////
    void setForce(const sf::Vector2f& force);

    ////////////////////////////////////////////////////////////
    /// \brief add force
    ///
    /// \param force the new extra force
    ////////////////////////////////////////////////////////////
    void addForce(const sf::Vector2f& force);

    ////////////////////////////////////////////////////////////
    /// \brief return the force
    ///
    /// \return the force
    ////////////////////////////////////////////////////////////
    sf::Vector2f getForce() const;

    ////////////////////////////////////////////////////////////
    /// \brief inverse the force
    ////////////////////////////////////////////////////////////
    void inverseForce();

    /// current position
    sf::Vector2f currPos;
    /// previous position
    sf::Vector2f prevPos;
private:
    /// force
    sf::Vector2f force_;
    /// mass
    float mass_;
    /// friction
    float friction_;
};

inline sf::Vector2f PointMass::getForce() const
{
    return force_;
}

}
}

#endif // BLOB_POINTMASS_H
