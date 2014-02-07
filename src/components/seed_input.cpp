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

#include <sstream>

#include "seed_input.h"
#include "common/events.h"
#include "math/random.h"

namespace beblob
{
namespace component
{

SeedInput::SeedInput(ecs::EventManager::Ptr event, const sf::Vector2f &position) :
    Widget(event)
{
    setSize(sf::Vector2f(100, 30));
    setPosition(position);
    filter_ = L"0123456789";
    limit_ = 5;
    math::Random rng;
    std::stringstream ss;
    for(int i=0;i<5;i++)
        ss << rng.next(9);
    input_ = ss.str().c_str();
    invalidate();
}

void SeedInput::onClick(const sf::Vector2f& ui, const sf::Vector2f& map)
{
    selected_ = true;
    invalidate();
}

void SeedInput::onMouseOut()
{
    selected_ = false;
    invalidate();
}

void SeedInput::invalidate()
{
    clear();

    std::string str = input_.toAnsiString();
    std::istringstream iss(str);
    int n;
    if (iss >> n)
    {
        events_->emit<event::UI::SelectSeed>(n);
    }

    rectangle(sf::Color(200, 0, 0));
    if(selected_)
        rectangle(sf::Color(200, 200, 200), false);
    text(sf::Vector2f(10, 0), input_, sf::Color(0, 0, 0));
}

void SeedInput::onUnicode(sf::Uint32 unicode)
{
    if(selected_ == false) return ;
    if(unicode == 8) return ;
    if(input_.getSize() >= limit_) return ;
    sf::String s;
    s.insert(0, unicode);
    std::wstring ws = s.toWideString();
    if(filter_.size() == 0 || filter_.find(ws) != std::wstring::npos)
    {
        input_.insert(input_.getSize(), unicode);
        invalidate();
    }
}

void SeedInput::onKey(sf::Keyboard::Key key)
{
    if(selected_ == false) return ;
    if(key == sf::Keyboard::BackSpace)
    {
        if(input_.getSize() > 0)
        {
            input_.erase(input_.getSize() - 1);
            invalidate();
        }
    }
}
}

}
