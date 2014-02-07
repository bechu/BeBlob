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

#ifndef BEBLOB_COMPONENT_BLOB_AI_H
#define BEBLOB_COMPONENT_BLOB_AI_H

#include "ecs/component.h"
#include "ai/astar.h"
#include "ai/job.h"
#include "blob_solid.h"
#include "world.h"

namespace beblob
{
namespace component
{

class BlobAI
        : public ecs::Component<BlobAI>
{
public:
    /// a blob shared pointer definition
    typedef std::shared_ptr<BlobAI> Ptr;

    ////////////////////////////////////////////////////////////
    /// \brief ai creation
    ///
    /// \param id team id
    ////////////////////////////////////////////////////////////
    BlobAI(component::Team::Id id);

    ////////////////////////////////////////////////////////////
    /// \brief update the component (manage jobs)
    ///
    /// \param dt diff time
    /// \param physic blob solid
    /// \param world game's world
    ////////////////////////////////////////////////////////////
    void update(double dt,
                BlobSolid &physic,
                World& world);

    ////////////////////////////////////////////////////////////
    /// \brief game loop call every x seconds
    ///
    /// \param blob this blob entity
    /// \param world game's world
    /// \param event global event manager
    ////////////////////////////////////////////////////////////
    utils::Job::Action gameLoop(ecs::Entity blob,
                  World& world,
                  ecs::EventManager::Ptr events);

    ////////////////////////////////////////////////////////////
    /// \brief select the blob
    ////////////////////////////////////////////////////////////
    bool& select();

    ////////////////////////////////////////////////////////////
    /// \brief return the blob select state
    ///
    /// \return bool true : blob selected
    ////////////////////////////////////////////////////////////
    bool select() const;

    ////////////////////////////////////////////////////////////
    /// \brief give a job to this blob
    ///
    /// \param job job pointer
    ////////////////////////////////////////////////////////////
    void giveJob(utils::Job::Ptr job);

    ////////////////////////////////////////////////////////////
    /// \brief return the current job
    ///
    /// \return current job
    ////////////////////////////////////////////////////////////
    utils::Job::Ptr getJob();

    ////////////////////////////////////////////////////////////
    /// \brief cancel the current job
    ///
    /// \param physic blob solid
    ////////////////////////////////////////////////////////////
    void cancel(BlobSolid& physic);

    ////////////////////////////////////////////////////////////
    /// \brief cancel the current job
    ///
    /// \param physic blob solid
    ////////////////////////////////////////////////////////////
    void stop(BlobSolid& physic);

    ////////////////////////////////////////////////////////////
    /// \brief test if the blob is busy
    ///
    /// \return true the blob is busy
    ////////////////////////////////////////////////////////////
    bool isBusy() const;

    ////////////////////////////////////////////////////////////
    /// \brief the blob collide the world
    ///
    /// \param world game'es world
    ///
    /// \return true collide
    ////////////////////////////////////////////////////////////
    bool collide(World& world);

    ////////////////////////////////////////////////////////////
    /// \brief return the path of the current job
    ///
    /// \return a list of position, empty if no job
    ////////////////////////////////////////////////////////////
    utils::AStar::Path getPath();

    ////////////////////////////////////////////////////////////
    /// \brief return the path of the current job
    ///
    /// \return a list of position, empty if no job
    ////////////////////////////////////////////////////////////
    sf::Vector2i getGoal() const;

    ////////////////////////////////////////////////////////////
    /// \brief return the state of the blob (life)
    ///
    /// \return a value between 0:1
    ////////////////////////////////////////////////////////////
    float state() const;

    ////////////////////////////////////////////////////////////
    /// \brief the blob eat
    ////////////////////////////////////////////////////////////
    void eat();

    ////////////////////////////////////////////////////////////
    /// \brief get the focus status of this blob
    ///
    /// \return just a boolean
    ////////////////////////////////////////////////////////////
    bool focus() const;

    ////////////////////////////////////////////////////////////
    /// \brief get the focus status of this blob by ref
    ///
    /// \return a ref to a bool
    ////////////////////////////////////////////////////////////
    bool& focus();

    ////////////////////////////////////////////////////////////
    /// \brief return the team id of this blob
    ///
    /// \return team id !
    ////////////////////////////////////////////////////////////
    component::Team::Id getTeamId() const;
private:
    ////////////////////////////////////////////////////////////
    /// \brief manage the list of jobs, pick a job if necessary
    ///
    /// \param physic blob physic instance
    /// \param world the semantic world
    ////////////////////////////////////////////////////////////
    void manage_job(BlobSolid& physic, World& world);

    /// the mouse is on a blob or not ;)
    bool focus_;
    /// job have to handle
    utils::Job::Ptr job_;
    /// list of jobs for this blob
    utils::Jobs jobs_;
    double timeout_ = 0;
    /// true if blob is selected
    bool selected_ = false;
    /// state of the blob
    float state_ = .5f;
    /// true if the mouse is on over the blob
    sf::Vector2f last_pos_;
    /// the team id of the blob
    component::Team::Id team_;
};

inline void BlobAI::eat()
{
    state_ += 0.05;
}

inline bool BlobAI::focus() const
{
    return focus_;
}

inline bool& BlobAI::focus()
{
    return focus_;
}

inline component::Team::Id BlobAI::getTeamId() const
{
    return team_;
}

inline float BlobAI::state() const
{
    return state_;
}

inline sf::Vector2i BlobAI::getGoal() const
{
    if(job_)
        return job_->getGoal();
    return sf::Vector2i(-1, -1);
}

inline bool BlobAI::collide(World& world)
{
    if(job_)
        return job_->collide(world);
    return false;
}

inline bool BlobAI::isBusy() const
{
    return static_cast<bool>(job_) || jobs_.size() > 0;
}

inline bool& BlobAI::select()
{
    return selected_;
}

inline bool BlobAI::select() const
{
    return selected_;
}

inline utils::Job::Ptr BlobAI::getJob()
{
    return job_;
}

}
}

#endif // BEBLOB_COMPONENT_BLOB_AI_H
