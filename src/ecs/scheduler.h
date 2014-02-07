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

#ifndef BEBLOB_ECS_SCHEDULER_H
#define BEBLOB_ECS_SCHEDULER_H

#include <functional>
#include <chrono>
#include <future>
#include <queue>
#include <thread>
#include <memory>

#include "ecs/event.h"

namespace ecs
{

class Scheduler
        : public Receiver
{
public:
    struct Task
    {
        ecs::BaseEvent::Ptr event;
        ecs::TypeId type;
        std::chrono::system_clock::time_point time;
        std::chrono::system_clock::duration interval;
        bool repeat = false;

        Task() {}

        Task(const Task& at)
        {
            event = at.event;
            type = at.type;
            time =  at.time;
            interval = at.interval;
            repeat = at.repeat;
        }

        bool operator<(const Task& rhs) const
        {
            return time > rhs.time;
        }
    };

    void update();

    void addJob(const Task& task);
private:
    std::priority_queue<Task> tasks;
};

}

#endif // BEBLOB_ECS_SCHEDULER_H
