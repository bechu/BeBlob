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

#ifndef BEBLOB_COMPONENT_SEED_INPUT_H
#define BEBLOB_COMPONENT_SEED_INPUT_H

#include "graphic/widget.h"
#include "ecs/event.h"
#include "ecs/ecs.h"

namespace beblob
{
namespace component
{

class SeedInput
        : public Widget
{
public:
    SeedInput(ecs::EventManager::Ptr event, const sf::Vector2f& position);
    void onClick(const sf::Vector2f& ui, const sf::Vector2f& map);
    void onUnicode(sf::Uint32 unicode);
    void onKey(sf::Keyboard::Key key);
    void onMouseOut();
private:
    void invalidate();
    sf::String input_;
    uint32_t limit_;
    std::wstring filter_;
    bool selected_ = false;
};

}
}

#endif // BEBLOB_COMPONENT_SEED_INPUT_H

