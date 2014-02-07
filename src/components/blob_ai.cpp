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

#include "blob_ai.h"
#include "world.h"
#include "math/vector.h"
#include "math/random.h"
#include "land_physic.h"
#include "common/events.h"

namespace beblob
{
namespace component
{

BlobAI::BlobAI(component::Team::Id id) :
    team_(id)
{
}

void BlobAI::update(double dt, BlobSolid& physic, World &world)
{
    manage_job(physic, world);
    if(job_)
    {
        // yes -> so continue to do the job
        if(!job_->doStuff(dt, physic, world))
            cancel(physic);
    }

    sf::Vector2f pos = physic.getPosition();
    if(std::abs(pos.x-last_pos_.x) < 0.3
            && std::abs(pos.y-last_pos_.y) < 0.3)
    {
        timeout_ += dt;
    }
    else
        timeout_ = 0;
    last_pos_ = pos;
    if(timeout_ > 5000)
        cancel(physic);
}

void BlobAI::stop(BlobSolid& physic)
{
    physic.setForce(sf::Vector2f());
    job_.reset();
}

void BlobAI::manage_job(BlobSolid& physic, World& world)
{
    // if this blob has no job
    if(!job_)
    {
        // and some job waiting in the queue
        while(jobs_.size() > 0)
        {
            // take the first job & remove it from head stack
            job_ = jobs_.front();
            jobs_.pop();
            // if the job is still valid
            if(job_->checkValidity(world) == true)
            {
                // try to initiate it (maybe not accessible for the moment)
                // in case of true, do it
                if(job_->initiate(physic, world) == false)
                    job_.reset();
            }
        }
    }
}

void BlobAI::giveJob(utils::Job::Ptr job)
{
    timeout_ = 0;
    jobs_.push(job);
}

utils::Job::Action BlobAI::gameLoop(ecs::Entity blob,
                      World& world,
                      ecs::EventManager::Ptr events)
{
    state_ -= .005f;
    if(job_)
        return job_->gameLoop(blob, world, events);
    return utils::Job::kNothing;
}

void BlobAI::cancel(BlobSolid& physic)
{
    stop(physic);
    job_.reset();
}

utils::AStar::Path BlobAI::getPath()
{
    if(job_) return job_->getPath();
    return utils::AStar::Path();
}

}
}
