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

#include "sounds.h"
#include "sf/zip_stream.h"

namespace beblob
{
namespace system
{

void Sounds::configure()
{
    subscribe<event::Sound::Play>();

    subscribe<event::Game::New>();

    subscribe<event::UI::Pause,
            event::UI::Resume>();
}

void Sounds::initialize()
{
    sounds_.resize(30);
}

void Sounds::update(double dt)
{
}

void Sounds::receive(const event::Sound::Play& event)
{
    if(sprites_.find(event.Filename) == sprites_.end())
    {
        sprites_[event.Filename] = sf::SoundBuffer();
        sf::ZipStream zip = {"blob.pack"};
        zip.open(event.Filename);
        sprites_[event.Filename].loadFromStream(zip);
    }
    for(auto& sound : sounds_)
    {
        if(sound.getStatus() == sf::SoundSource::Stopped)
        {
            sound.setBuffer(sprites_[event.Filename]);
            sound.play();
            break;
        }
    }
}

void Sounds::receive(const event::UI::Pause& event)
{
    for(auto& sound : sounds_)
    {
        if(sound.getStatus() == sf::SoundSource::Playing)
        {
            paused_.push_back(&sound);
            sound.pause();
        }
    }
}

void Sounds::receive(const event::UI::Resume& event)
{
    for(auto& sound : paused_)
    {
        sound->play();
    }
    paused_.clear();
}

void Sounds::receive(const event::Game::New& event)
{
    for(auto& sound : sounds_)
        sound.stop();
}

}
}
