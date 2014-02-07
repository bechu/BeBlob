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

#ifndef BEBLOB_COMPONENT_LABEL_H
#define BEBLOB_COMPONENT_LABEL_H

#include "graphic/widget.h"
#include "ecs/event.h"
#include "ecs/ecs.h"

namespace beblob
{
namespace component
{

class Label
        : public Widget
{
public:
    Label(ecs::EventManager::Ptr event,
          const sf::Vector2f& position,
          const std::wstring &label,
          const sf::Color& color=sf::Color::White,
          uint32_t font_size=24);
};

}
}

#endif // BEBLOB_COMPONENT_LABEL_H
