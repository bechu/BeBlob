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

#ifndef BLOB_WARS_STICK_H
#define BLOB_WARS_STICK_H

#include "pointmass.h"

namespace beblob
{
namespace physic
{

////////////////////////////////////////////////////////////
/// \brief stick class
////////////////////////////////////////////////////////////
struct Stick
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief create a new stick
    ////////////////////////////////////////////////////////////
    Stick();

    ////////////////////////////////////////////////////////////
    /// \brief create a new stick
    ///
    /// \param aA the first mass point
    /// \param aB the second mass point
    ////////////////////////////////////////////////////////////
    Stick(PointMass& aA, PointMass& aB);

    ////////////////////////////////////////////////////////////
    /// \brief create a new stick from another one
    ///
    /// \param that original stick
    ////////////////////////////////////////////////////////////
    Stick(const Stick& that);

    ////////////////////////////////////////////////////////////
    /// \brief scale the stick
    ///
    /// \param scaleFactor the factor used to resize the stick
    ////////////////////////////////////////////////////////////
    void scale(float scaleFactor);

    ////////////////////////////////////////////////////////////
    /// \brief set the length of the stick
    ///
    /// \param length new length of the stick
    ////////////////////////////////////////////////////////////
    void setLength(float length);

    ////////////////////////////////////////////////////////////
    /// \brief update the stick
    ////////////////////////////////////////////////////////////
    void satisfy();

    ////////////////////////////////////////////////////////////
    /// \brief return the start mass point
    ///
    /// \return a
    ////////////////////////////////////////////////////////////
    const PointMass* begin() const;

    ////////////////////////////////////////////////////////////
    /// \brief return the final mass point
    ///
    /// \return b
    ////////////////////////////////////////////////////////////
    const PointMass* end() const;
private:
    /// a stick is composed of two point, a, b
    PointMass* a;
    /// second mass point of this stick
    PointMass* b;
    /// distance between mass point
    float lengthSquared;
};

inline const PointMass* Stick::begin() const
{
    return a;
}

inline const PointMass* Stick::end() const
{
    return b;
}

}
}

#endif // BLOB_WARS_STICK_H
