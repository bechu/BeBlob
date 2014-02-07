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

#ifndef BLOB_WARS_JOINT_H
#define BLOB_WARS_JOINT_H

#include "pointmass.h"

namespace beblob
{
namespace physic
{

////////////////////////////////////////////////////////////
/// \brief joint definition
////////////////////////////////////////////////////////////
class Joint
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Joint();

    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ///
    /// \param aA the begin of the joint
    /// \param aB the end of the joint
    /// \param aShortConst min size
    /// \param aLongConst max size
    ////////////////////////////////////////////////////////////
    Joint(PointMass& aA, PointMass& aB, float aShortConst, float aLongConst);

    ////////////////////////////////////////////////////////////
    /// \brief scale the joint
    ///
    /// \param scaleFactor scale
    ////////////////////////////////////////////////////////////
    void scale(float scaleFactor);

    ////////////////////////////////////////////////////////////
    /// \brief set new constraints
    ///
    /// \param aShortConst min size
    /// \param aLongConst max size
    ////////////////////////////////////////////////////////////
    void setConstraints(float aShortConst, float aLongConst);

    ////////////////////////////////////////////////////////////
    /// \brief update the joint
    ////////////////////////////////////////////////////////////
    void satisfy();
private:
    /// start mass point
    PointMass* a;
    /// end mass point
    PointMass* b;
    /// min size
    float shortConstSquared;
    /// max size
    float longConstSquared;
};

}
}

#endif // BLOB_WARS_JOINT_H
