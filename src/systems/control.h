/////////////////////////////////////////////////////////////////////////
/// BeBlob
/// Copyright (C) 2014 Jérôme Béchu
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version. ///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#ifndef BEBLOB_SYSTEM_CONTROL_H
#define BEBLOB_SYSTEM_CONTROL_H

#include "ecs/ecs.h"
#include "components/window.h"
#include "common/events.h"

namespace beblob
{
namespace system
{

class Control
        : public ecs::System<Control>
{
public:
    void configure();
    void initialize();
    void update(double dt);
    void release();

    void receive(const event::UI::Button& event);
    void receive(const event::UI::ColorPicker& event);
    void receive(const event::UI::SelectSeed& event);
    void receive(const event::UI::Pause& event);
    void receive(const event::UI::Resume& event);
    void receive(const event::Game::Start& event);
    void receive(const event::Game::New& event);
    void receive(const event::Game::End& event);
private:
    enum
    {
        kQuit,
        kLoad,
        kResume,
        kNew
    };
    ////////////////////////////////////////////////////////////
    /// \brief clear ui elements
    ////////////////////////////////////////////////////////////
    void clear_ui();
    void treatEventForGame(const sf::Event& event,
                           const sf::Vector2f &ui_position,
                           const sf::Vector2f &map_position);
    /// a pointer to window
    component::Window::Ptr window_;
    /// the gui entity
    ecs::Entity gui_;
    /// default blob player color
    sf::Color color_ = {255, 0, 0, 255};
    /// this is the current zoom
    double zoom_ = 1;
    /// Am i currently move the camera ?
    bool move_ = false;
    //// used to handle camera shifting
    sf::Vector2i last_pos;
    /// store the seed value choosen
    math::Random::Seed seed_;
    /// handle the exit button
    bool run_ = true;
    /// currently in game ?
    bool playing_ = false;
};

}
}

#endif // BEBLOB_SYSTEM_CONTROL_H
