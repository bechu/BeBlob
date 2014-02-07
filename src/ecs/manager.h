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

#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H

#include <memory>

#include "entity_manager.h"
#include "system.h"
#include "system_manager.h"
#include "common/unused.h"

namespace ecs
{

class Manager
        : public Receiver
{
public:
    struct Terminate : public Event<Terminate> {};
    Manager();

    template <typename S, typename ... Args>
    std::shared_ptr<S> addSystem(int priority, Args && ... args)
    {
      std::shared_ptr<S> s = std::make_shared<S>(std::forward<Args>(args) ...);
      s->setPriority(priority);
      sm_->add(s);
      return s;
    }

    void receive(const Terminate& UNUSED(event))
    {
        forever_ = false;
    }

    void configure()
    {
        sm_->configure();
    }

    void initialize()
    {
        sm_->initialize();
    }

    int run();

private:
    ecs::SystemManager::Ptr sm_;
    bool forever_ = true;
};

}

#endif // ECS_MANAGER_H

