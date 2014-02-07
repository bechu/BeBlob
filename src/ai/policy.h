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

#ifndef BEBLOB_AI_POLICY_H
#define BEBLOB_AI_POLICY_H

#include <memory>
#include "components/world.h"
#include "components/team.h"
#include "ai/job.h"

namespace beblob
{
namespace utils
{

////////////////////////////////////////////////////////////
/// \brief a task definition : loc & score
////////////////////////////////////////////////////////////
struct Task
{
    int32_t score;
    Task& operator=(const Task& rhs)
    {
        score = rhs.score;
        loc = rhs.loc;
        return *this;
    }
    bool operator<(const Task &rhs) const { return score < rhs.score; }
    sf::Vector2i loc;
    typedef std::vector<Task> List;
};

////////////////////////////////////////////////////////////
/// \brief member teams (copy of blob_ai)
////////////////////////////////////////////////////////////
struct Member
{
    sf::Vector2i position;
    sf::Vector2i goal;
    ecs::Entity::Id id;
    bool busy = false;
    double state = 0;
    std::shared_ptr<utils::Job> job;
};

////////////////////////////////////////////////////////////
/// \brief team defintion for expert thread
////////////////////////////////////////////////////////////
struct Team :
        public std::vector<Member>
{
    component::Team::Id id;
    std::vector<sf::Vector2i> getGoals();
    void sort_hungry();
};

////////////////////////////////////////////////////////////
/// \brief a global policy for blobs
////////////////////////////////////////////////////////////
class Policy
{
public:
    typedef std::shared_ptr<Policy> Ptr;
protected:
    virtual void proceed(component::World& world, utils::Team& team) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief find a task around a position
    ///
    /// \param world game's world
    /// \param types list of type seek for this task
    /// \param the origin of the research
    ///
    /// \return (-1, -1) in case of fail
    ////////////////////////////////////////////////////////////
    sf::Vector2i findTask(component::World& world,
                          const component::World::Cell::TypeList& types,
                          const sf::Vector2i& to, const sf::Vector2i &from,
                          int radius_max,
                          bool direct = true);
protected:
    bool isGoal(const sf::Vector2i& goal) const;
    void computeGoals(utils::Team& team);
    std::vector<sf::Vector2i> goals_;
private:
    ////////////////////////////////////////////////////////////
    /// \brief find a task for a specific radius
    ///
    /// \param world game's world
    /// \param types list of type seek for this task
    /// \param the origin of the research
    /// \param radius where exactly
    ///
    /// \return (-1, -1) in case of fail
    ////////////////////////////////////////////////////////////
    sf::Vector2i findOnRadius(component::World& world,
                              const component::World::Cell::TypeList& types,
                              const sf::Vector2i& to, const sf::Vector2i &from,
                              int radius, bool direct=true);

};

}
}

#endif // BEBLOB_AI_POLICY_H
