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

#ifndef BEBLOB_EVENTS_H
#define BEBLOB_EVENTS_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "ecs/event.h"
#include "ai/job.h"
#include "components/world.h"

namespace beblob
{
namespace event
{

struct UI
{
    struct Button
            : public ecs::Event<Button>
    {
        Button(int id) : ID(id) {}
        int ID;
    };
    struct ColorPicker
            : public ecs::Event<ColorPicker>
    {
        ColorPicker(const sf::Color& color) : Color(color)
        {
        }
        sf::Color Color;
    };
    struct SelectSeed
            : public ecs::Event<SelectSeed>
    {
        SelectSeed(math::Random::Seed seed) : Graine(seed) {}
        math::Random::Seed Graine;
    };
    struct Pause
            : public ecs::Event<Pause>
    {
        Pause(bool resume=true) : Resume(resume)
        {
        }
        bool Resume;
    };
    struct Resume
            : public ecs::Event<Resume>
    {
    };
};

struct Game
{
    struct Build
            : public ecs::Event<Build>
    {
        Build(int team_count, math::Random::Seed seed, const sf::Color& color)
            : TeamCount(team_count)
            , Graine(seed)
            , Color(color)
        {}
        int TeamCount;
        math::Random::Seed Graine;
        sf::Color Color;
    };
    struct New
            : public ecs::Event<New>
    {
    };
    struct Start
            : public ecs::Event<Start>
    {
    };
    struct End
            : public ecs::Event<End>
    {
        End(bool playerwin)
            : PlayerWin(playerwin)
        {
        }
        bool PlayerWin;
    };
};

struct World
{
    struct Shape
            : public ecs::Event<Shape>
    {
        Shape(const ecs::Entity& game, math::Random::Seed seed) :
            Game(game), Graine(seed) {}
        ecs::Entity Game;
        math::Random::Seed Graine;
    };
    struct MakeUp
            : public ecs::Event<MakeUp>
    {
        MakeUp(const ecs::Entity& game, math::Random::Seed seed)
            : Game(game)
            , Graine(seed)
        {}
        ecs::Entity Game;
        math::Random::Seed Graine;
    };
    struct DeleteBlock
            : public ecs::Event<DeleteBlock>
    {
        DeleteBlock(const sf::Vector2i& position)
            : Position(position)
        {
        }
        sf::Vector2i Position;
    };
    struct AddBlock
            : public ecs::Event<AddBlock>
    {
        AddBlock(const sf::Vector2i& position)
            : Position(position)
        {
        }
        sf::Vector2i Position;
    };
};

struct Blob
{
    struct New
            : public ecs::Event<New>
    {
        New(const sf::Vector2f& position, int team, const sf::Color& color):
            Position(position),
            Team(team),
            Color(color)
        {
        }
        sf::Vector2f Position;
        int Team;
        sf::Color Color;
    };
    struct Job : public ecs::Event<Job>
    {
        Job(const ecs::Entity& blob, const sf::Vector2i& pos, bool left=false) :
            Blob(blob),
            Pos(pos),
            Left(left)
        {
        }
        ecs::Entity Blob;
        sf::Vector2i Pos;
        bool Left;
    };
    struct Remove : public ecs::Event<Remove>
    {
        Remove(const ecs::Entity& blob) : Blob(blob)
        {
        }
        ecs::Entity Blob;
    };
    struct Split : public ecs::Event<Split>
    {
        Split(const ecs::Entity& blob) : Blob(blob)
        {
        }
        ecs::Entity Blob;
    };
    struct Collide : public ecs::Event<Collide>
    {
        Collide(const ecs::Entity& blob, const ecs::Entity& land, const sf::Vector2f& position)
            : Blob(blob), Land(land), Position(position)
        {
        }
        ecs::Entity Blob;
        ecs::Entity Land;
        sf::Vector2f Position;
    };

    struct  BuildBlock : ecs::Event<BuildBlock>
    {
        BuildBlock(const sf::Vector2i& position) : Position(position)
        {
        }
        sf::Vector2i Position;
    };

    struct Stop : ecs::Event<Stop> {};
};

struct Mouse
{
    struct Click : public ecs::Event<Click>
    {
        Click(const sf::Vector2f& pos, sf::Mouse::Button button) : Position(pos), Button(button) {}
        sf::Vector2f Position;
        sf::Mouse::Button Button;
    };

    struct DoubleClick : public ecs::Event<DoubleClick>
    {
        DoubleClick(const sf::Vector2f& pos) : Position(pos) {}
        sf::Vector2f Position;
    };

    struct Move : public ecs::Event<Move>
    {
        Move(const sf::Vector2f& pos) : Position(pos) {}
        sf::Vector2f Position;
    };

    struct Release : public ecs::Event<Release>
    {
        Release(const sf::Vector2f& pos) : Position(pos) {}
        sf::Vector2f Position;
    };

};

struct OnKey : public ecs::Event<OnKey>
{
    OnKey(sf::Keyboard::Key key) : Key(key)
    {}
    sf::Keyboard::Key Key;
};

struct Particle : public ecs::Event<Particle>
{
    struct Slap : public ecs::Event<Slap>
    {
        Slap(const sf::Vector2f& position) : Position(position)
        {
        }
        sf::Vector2f Position;
    };
    struct Fireworks :
            public ecs::Event<Fireworks>
    {
        Fireworks(const sf::Vector2f& position, const sf::Color& color) :
            Position(position),
            Color(color)
        {
        }
        sf::Vector2f Position;
        sf::Color Color;
    };
    struct Smoke :
            public ecs::Event<Smoke>
    {
        Smoke(const sf::Vector2f& position) : Position(position)
        {
        }
        sf::Vector2f Position;
    };
};

struct Sound
        : public ecs::Event<Sound>
{
    struct Play
            : public ecs::Event<Play>
    {
        Play(const std::string& filename)
            : Filename(filename)
        {
        }
        std::string Filename;
    };
};

}
}

#endif // BEBLOB_EVENTS_H

