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

#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <set>
#include <vector>
#include <chrono>

#include "common/unused.h"
#include "event.h"
#include "entity_manager.h"
#include "scheduler.h"

namespace ecs
{

class SystemManager;

class BaseSystem
{
public:
    typedef std::shared_ptr<BaseSystem> Ptr;
    BaseSystem() {}
    void accept();
    //void configure(component);
    //void configure(event);
    virtual void configure() = 0;
    virtual void initialize() = 0;
    virtual void update(double dt) = 0;
    virtual void release() = 0;
    //virtual void update(Events& events, double dt) = 0;

    int getPriority() const
    {
        return priority_;
    }
    void setPriority(int p)
    {
        priority_ = p;
    }

    template <typename C>
    void accept_once()
    {
        accept_.push_back(C::getTypeId());
    }

    template<typename...>
    struct typelist{};

    template<typename T, typename ... Rest>
    void call(typelist<T,Rest...>)
    {
        accept_once<T>();
        call(typelist<Rest...>());
    }

    void call(typelist<>) {}

    template<typename...classes>
    void accept()
    {
        call(typelist<classes...>());
    }

protected:
    int priority_ = 0;
private:
    std::vector<TypeId> accept_;
};


template <typename Derived>
class System : public BaseSystem, public Receiver
{
public:
    System()
    {
    }
    virtual ~System() {}

    static TypeId getTypeId()
    {
        return ClassTypeId<BaseSystem>::getTypeId<Derived>();
    }

    template <typename E>
    void subscribe_once()
    {
        Derived * d = static_cast<Derived*>(this);
        event_manager_->subscribe<E>(*d);
    }

    template<typename...>
    struct typelist_event{};

    template<typename E, typename ... Rest>
    void call_event(typelist_event<E,Rest...>)
    {
        subscribe_once<E>();
        call_event(typelist_event<Rest...>());
    }

    void call_event(typelist_event<>) { }

    template<typename...classes>
    void subscribe()
    {
        call_event(typelist_event<classes...>());
    }

    Entity createEntity()
    {
        return entity_manager_->createEntity();
    }

    EntityManager::Ptr entity_manager_;

    template <typename C>
    Entity::List entities()
    {
        return entity_manager_->entities<C>();
    }

    template <typename E, typename ... Args>
    void at(std::chrono::system_clock::duration d, Args && ... args)
    {
        Scheduler::Task task;
        task.event = std::make_shared<E>(std::forward<Args>(args) ...);
        task.type = E::getTypeId();
        task.interval = d;
        task.repeat = false;
        entity_manager_->addJob(task);
    }

    template <typename E, typename ... Args>
    void every(std::chrono::system_clock::duration d, Args && ... args)
    {
        Scheduler::Task task;
        task.event = std::make_shared<E>(std::forward<Args>(args) ...);
        task.type = E::getTypeId();
        task.interval = d;
        task.repeat = true;
        entity_manager_->addJob(task);
    }

private:
    friend class SystemManager;

    std::weak_ptr<SystemManager> manager_;
};

}

#endif // ECS_SYSTEM_H

