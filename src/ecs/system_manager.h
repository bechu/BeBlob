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

#ifndef ECS_SYSTEM_MANAGER_H
#define ECS_SYSTEM_MANAGER_H

#include <memory>

#include "system.h"
#include "event.h"
#include "entity_manager.h"

namespace ecs
{

class SystemManager
        : public std::enable_shared_from_this<SystemManager>
{
public:

    typedef std::shared_ptr<SystemManager> Ptr;
    static Ptr make()
    {
        Ptr sm = std::make_shared<SystemManager>();
        sm->entity_manager_ = std::shared_ptr<EntityManager>(new EntityManager());
        sm->entity_manager_->event_manager_ = std::make_shared<EventManager>();
        return sm;
    }

    template <typename S>
    void add(std::shared_ptr<S> system)
    {
        system->manager_ = shared_from_this();
        system->entity_manager_ = entity_manager_;
        system->event_manager_ = entity_manager_->event_manager_;
        //system->subscribe<EntityUpdate>();
        systems_.insert(std::make_pair(S::getTypeId(), system));
        order_.push_back(system);
    }

    template <typename S, typename ... Args>
    std::shared_ptr<S> add(Args && ... args)
    {
        std::shared_ptr<S> s = std::make_shared<S>(std::forward<Args>(args) ...);
        add(s);
        return s;
    }

    template <typename S>
    std::shared_ptr<S> system()
    {
        auto it = systems_.find(S::getTypeId());
        assert(it != systems_.end());
        return it == systems_.end()
                ? std::shared_ptr<S>()
                : std::static_pointer_cast<S>(it->second);
    }

    void update(double dt);
    void initialize();
    void configure();
    void release();
    void populate(const Entity& e, TypeId f);

    std::shared_ptr<EntityManager> entity_manager_;
private:
    std::unordered_map<TypeId, BaseSystem::Ptr> systems_;
    std::vector<BaseSystem::Ptr> order_;
};

}

#endif // ECS_SYSTEM_MANAGER_H

