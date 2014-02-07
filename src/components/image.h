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

#ifndef BEBLOB_COMPONENT_IMAGE_H
#define BEBLOB_COMPONENT_IMAGE_H

#include "graphic/widget.h"
#include "ecs/event.h"
#include "ecs/ecs.h"

namespace beblob
{
namespace component
{

////////////////////////////////////////////////////////////
/// \brief image widget
////////////////////////////////////////////////////////////
class Image
        : public Widget
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ///
    /// \param event event manager used to populate event to systems
    /// \param position where to place the picture
    /// \param image path of the image
    ////////////////////////////////////////////////////////////
    Image(ecs::EventManager::Ptr event,
          const sf::Vector2f& position,
          const std::string& image);

    ////////////////////////////////////////////////////////////
    /// \brief give a speed rotation
    ///
    /// \param theta speed rotation
    ////////////////////////////////////////////////////////////
    void setSpeedRotation(double theta);
private:
    void update(double dt);

    /// texture hold the image
    sf::Texture texture_;
    /// the speed use for the rotation
    double speed_;
};

inline void Image::setSpeedRotation(double theta)
{
    speed_ = theta;
}

}
}

#endif // BEBLOB_COMPONENT_IMAGE_H
