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

#ifndef BEBLOB_COMPONENT_TEAM_H
#define BEBLOB_COMPONENT_TEAM_H

#include "ecs/entity.h"
#include <SFML/System/Vector2.hpp>

namespace beblob
{
namespace component
{

class Team
        : public ecs::Component<Team>
{
public:
    /// shared pointer definition
    typedef std::shared_ptr<Team> Ptr;
    /// team id type
    typedef uint32_t Id;

    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ////////////////////////////////////////////////////////////
    Team();

    ////////////////////////////////////////////////////////////
    /// \brief get the team id
    ///
    /// \return the id
    ////////////////////////////////////////////////////////////
    Id getID() const;

    ////////////////////////////////////////////////////////////
    /// \brief add a new blob
    ///
    /// \param blob the blob
    ////////////////////////////////////////////////////////////
    void addMember(const ecs::Entity& blob);

    ////////////////////////////////////////////////////////////
    /// \brief remove a blob
    ///
    /// \param blob the blob
    ////////////////////////////////////////////////////////////
    void removeMember(const ecs::Entity& blob);

    ////////////////////////////////////////////////////////////
    /// \brief return a member
    ///
    /// \param index which one
    ///
    /// \return an entity
    ////////////////////////////////////////////////////////////
    ecs::Entity& getMember(uint32_t index);

    ////////////////////////////////////////////////////////////
    /// \brief return the number of blob
    ////////////////////////////////////////////////////////////
    size_t size() const;

    ////////////////////////////////////////////////////////////
    /// \brief return the number of team
    ////////////////////////////////////////////////////////////
    static Id countTeam();

    ////////////////////////////////////////////////////////////
    /// \brief reset the static team id counter
    ////////////////////////////////////////////////////////////
    static void reset();
private:
    ////////////////////////////////////////////////////////////
    /// \brief static method use to get an id
    ///
    /// \return Id team id
    ////////////////////////////////////////////////////////////
    static Id getNextId();

    /// the list of blob for the bot team
    std::vector<ecs::Entity> members_;
    /// the id of this team
    Id id_;
    /// a static counter for team::id
    static Id nextId_;
};

inline Team::Id Team::getID() const
{
    return id_;
}

inline Team::Id Team::countTeam()
{
    return nextId_;
}

inline Team::Id Team::getNextId()
{
    Team::Id id = nextId_;
    ++nextId_;
    return id;
}

inline size_t Team::size() const
{
    return members_.size();
}

}
}

#endif // BEBLOB_COMPONENT_TEAM_H
