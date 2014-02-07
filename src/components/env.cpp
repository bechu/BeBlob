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

#include "env.h"
#include "world.h"
#include "land_physic.h"

namespace beblob
{
namespace component
{

Env::Env() :
    rect_(0, 0, World::kWidth*LandSolid::kSize, World::kHeight*LandSolid::kSize)
{
}

bool Env::collide(sf::Vector2f& pos) const
{
    bool collided = false;
    if(pos.x < rect_.left)
    {
      pos.x = rect_.left;
      collided = true;
    }
    else if(pos.x > rect_.width)
    {
      pos.x = rect_.width;
      collided = true;
    }
    if(pos.y < rect_.top)
    {
      pos.y = rect_.top;
      collided = true;
    }
    else if(pos.y > rect_.height)
    {
      pos.y = rect_.height;
      collided = true;
    }
    return collided;
}

}
}
