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

#ifndef BEBLOB_SOUNDS_H
#define BEBLOB_SOUNDS_H

#include <map>
#include <SFML/Audio.hpp>

#include "ecs/ecs.h"
#include "common/events.h"

namespace beblob
{
namespace system
{

class Sounds
        : public ecs::System<Sounds>
{
public:
    void configure();
    void initialize();
    void update(double dt);
    void release() {}

    void receive(const event::Sound::Play& event);

    void receive(const event::UI::Pause& event);
    void receive(const event::UI::Resume& event);

    void receive(const event::Game::New& event);
private:
    /// a list of alreay used sound
    std::map<std::string, sf::SoundBuffer> sprites_;
    /// list of sound
    std::vector<sf::Sound> sounds_;
    /// in case of pause use paused to know
    /// which sound played
    std::vector<sf::Sound*> paused_;
};

}
}

#endif // BEBLOB_SOUNDS_H
