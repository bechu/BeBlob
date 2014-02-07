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

#ifndef BEBLOB_LOGIC_WORLD_H
#define BEBLOB_LOGIC_WORLD_H

#include <memory>
#include <cstring>

#include "ecs/component.h"
#include "math/random.h"
#include "components/team.h"

namespace beblob
{
namespace component
{

////////////////////////////////////////////////////////////
/// \brief the world class
///        contains a grid of cells
////////////////////////////////////////////////////////////
class World
        : public ecs::Component<component::World>
{
public:
    /// width of the world
    const static int kWidth = 20;

    /// height of the world
    const static int kHeight = 20;

    /// the world shared pointer definition
    typedef std::shared_ptr<component::World> Ptr;

    /// the world weak pointer definition
    typedef std::weak_ptr<component::World> WeakPtr;

    /// cell definition
    // a cell can be swith to :
    // nothing to wall
    // wall to nothing
    // home to nothing
    // food to nothing
    struct Cell
    {
        /// type of cell
        typedef enum {kUnbreakable, kNothing, kWall, kFood, kHome} Type;

        typedef std::vector<Type> TypeList;

        /// the default value of a wall resistance
        constexpr static int kDefaultResistance = 4;

        /// the default value of house resistance
        constexpr static int kDefaultResistanceForHome = 10;

        /// the default quantity of food
        constexpr static int kDefaultFood = 50;

        Cell()
        {
            type = kNothing;
            Resistance = kDefaultResistance;
            Build = 0;
        }

        /// type
        Type type = kNothing;

        int Resistance; // current value of the cell
        int Food; // current food
        int Build = 0; // current building

        ////////////////////////////////////////////////////////////
        /// \brief test if this cell is collisionable
        ///
        /// \return true if collisionable
        ////////////////////////////////////////////////////////////
        bool isCollisionable() const;
        Cell& operator = (const Cell& rhs)
        {
            type = rhs.type;
            Food = rhs.Food;
            Build = rhs.Build;
            return *this;
        }
    };

    struct Observer
    {
        /// a observer shared pointer defintion
        typedef std::shared_ptr<Observer> Ptr;
        /// a callback call each time the world will be update
        virtual void updated(const sf::Vector2i& c,
                             const component::World::Cell& cell) = 0;
    };

    ////////////////////////////////////////////////////////////
    /// \brief world constructor : empty world
    ////////////////////////////////////////////////////////////
    explicit World();

    ////////////////////////////////////////////////////////////
    /// \brief build the world
    ///
    /// \paramseed the gameseed
    ////////////////////////////////////////////////////////////
    void create(math::Random::Seed seed, uint32_t player_count);

    ////////////////////////////////////////////////////////////
    /// \brief return a const cell
    ///
    /// \param c the coordinate of the cell
    ///
    /// \return the world cell (const)
    ////////////////////////////////////////////////////////////
    //component::World::Cell cell(const sf::Vector2i& c) const;

    ////////////////////////////////////////////////////////////
    /// \brief set values of a cell
    ///
    /// \param c the coordinate of the cell
    /// \param cell new values
    ////////////////////////////////////////////////////////////
    void set(const sf::Vector2i& c, const Cell &cell);

    ////////////////////////////////////////////////////////////
    /// \brief set the type of a cell
    ///
    /// \param c the coordinate of the cell
    /// \param type the new type
    ////////////////////////////////////////////////////////////
    void setType(const sf::Vector2i& c, component::World::Cell::Type type);

    ////////////////////////////////////////////////////////////
    /// \brief set the new resistance of a cell
    ///
    /// \param c the coordinate of the cell
    /// \param resistance the new resistance
    ////////////////////////////////////////////////////////////
    void setResistance(const sf::Vector2i& c, int resistance);

    ////////////////////////////////////////////////////////////
    /// \brief set the new level of food
    ///
    /// \param c the coordinate of the cell
    /// \param food the new value
    ////////////////////////////////////////////////////////////
    void setFood(const sf::Vector2i& c, int food);

    ////////////////////////////////////////////////////////////
    /// \brief set the new value for the current building
    ///
    /// \param c the coordinate of the cell
    /// \param build the new value to used
    ////////////////////////////////////////////////////////////
    void setBuild(const sf::Vector2i& c, int build);

    ////////////////////////////////////////////////////////////
    /// \brief return the home position
    ///
    /// \return the home coordinate
    ////////////////////////////////////////////////////////////
    const sf::Vector2i getHome(component::Team::Id index) const;

    const int getTeamIndex(const sf::Vector2i& home) const;

    ////////////////////////////////////////////////////////////
    /// \brief convert a screen position to map position
    ///
    /// \param pos the screen position
    ///
    /// \return the map position
    ////////////////////////////////////////////////////////////
    static sf::Vector2i screen2map(const sf::Vector2f& pos);

    ////////////////////////////////////////////////////////////
    /// \brief convert a map position to screen position
    ///
    /// \param pos the map position
    ///
    /// \return the screen position
    ////////////////////////////////////////////////////////////
    static sf::Vector2f map2screen(const sf::Vector2i& pos);


    ////////////////////////////////////////////////////////////
    /// \brief add a new world observer
    ///
    /// \param observer the instance used
    ////////////////////////////////////////////////////////////
    void addObserver(World::Observer::Ptr observer);


    ////////////////////////////////////////////////////////////
    /// \brief copy a world
    ///
    /// \param rhs the world used to copy
    ///
    /// \return the memory copy
    ////////////////////////////////////////////////////////////
    World& operator= (const World& rhs);

    World::Cell operator()(int x, int y) const;
    World::Cell operator()(const sf::Vector2i& c) const;
private:
    ////////////////////////////////////////////////////////////
    /// \brief avert observers
    ////////////////////////////////////////////////////////////
    void avert_observers(const sf::Vector2i &c);

    ////////////////////////////////////////////////////////////
    /// \brief return a cell
    ///
    /// \param c the coordinate of the cell
    ///
    /// \return the world cell
    ////////////////////////////////////////////////////////////
    component::World::Cell& getCell(const sf::Vector2i& c);

    component::World::Cell getCell(const sf::Vector2i& c) const;

    ////////////////////////////////////////////////////////////
    /// \brief generate the world
    ////////////////////////////////////////////////////////////
    void generate(math::Random::Seed seed);

    ////////////////////////////////////////////////////////////
    /// \brief define the position of the home
    ////////////////////////////////////////////////////////////
    void cleanHome(const sf::Vector2i &home);

    bool farFromOthers(const sf::Vector2i& home, int32_t count);

    /// the cells
    std::vector<Cell> cells_;

    /// a random generator
    math::Random rng_;

    /// the list of observers
    std::vector<World::Observer::Ptr> observers_;

    static component::World::Cell dummy_;

    /// home of each team
    std::vector<sf::Vector2i> homes_;
};

inline World::Cell World::operator()(int x, int y) const
{
    return getCell(sf::Vector2i(x, y));
}

inline World::Cell World::operator()(const sf::Vector2i& c) const
{
    return getCell(c);
}

inline const sf::Vector2i World::getHome(component::Team::Id index) const
{
    assert(index<homes_.size());
    return homes_[index];
}

inline const int World::getTeamIndex(const sf::Vector2i& home) const
{
   auto it = std::find(homes_.begin(), homes_.end(), home);
   if(it != homes_.end())
       return it - homes_.begin();
   return -1;
}

}
}

#endif // BEBLOB_LOGIC_WORLD_H
