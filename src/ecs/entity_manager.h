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

#ifndef ECS_ENTITY_MANAGER_H
#define ECS_ENTITY_MANAGER_H

#include <bitset>
#include <memory>
#include <vector>
#include <list>

#include "entity.h"
#include "event.h"
#include "scheduler.h"

namespace ecs
{

namespace event
{
}

class EntityManager
        : public std::enable_shared_from_this<EntityManager>, public Scheduler
{
public:
    static const int kMaxComponents = 100;
    typedef std::shared_ptr<EntityManager> Ptr;

    EntityManager();

    Entity createEntity();
    void destroy(Entity::Id id);

    template <typename C>
    void remove(const Entity::Id& id, unsigned int index)
    {
        if(checkContainer<C>(id) == false)
            return ;
        std::shared_ptr<C> component = std::static_pointer_cast<C>(entity_components_.at(C::getTypeId()).at(id).at(index));
        component.reset();
        entity_components_.at(C::getTypeId()).at(id).erase(
                    entity_components_.at(C::getTypeId()).at(id).begin() + index);
        // std::shared_ptr<C> component = std::static_pointer_cast<C>(entity_components_.at(C::getTypeId()).at(id));
        //entity_components_.at(C::getTypeId()).at(id).at(index).reset();
    }

    template <typename C>
    std::shared_ptr<C> component(const Entity::Id& id, unsigned int index=0)
    {
        if(checkContainer<C>(id) == false)
        {
            return std::shared_ptr<C>();
        }
        if(entity_components_.at(C::getTypeId()).at(id).size() <= index)
            return std::shared_ptr<C>();
        return std::static_pointer_cast<C>(entity_components_.at(C::getTypeId()).at(id)[index]);
    }

    template <typename C>
    unsigned int count(const Entity::Id& id)
    {
        if(checkContainer<C>(id) == false)
            return 0;
        return entity_components_.at(C::getTypeId()).at(id).size();
    }

    template <typename C>
    inline bool checkContainer(const Entity::Id& id)
    {
        if(entity_components_.size() == 0) return false;
        if (C::getTypeId() >= entity_components_.size())
        {
            return false;
        }
        if (id >= entity_components_.at(C::getTypeId()).size())
        {
            return false;
        }
        return true;
    }

    template <typename C>
    std::shared_ptr<C> assign(const Entity::Id& id, std::shared_ptr<C> component);

    template <typename C>
    Entity::List entities()
    {
        Entity::List list;
        if(C::getTypeId() >= entity_components_.size())
        {
            return list;
        }
        auto entities = entity_components_.at(C::getTypeId());
        for(Entity::Id i=0;i<entities.size();++i)
        {
            if(entities[i].size() > 0)
            {
                list.push_back(Entity(shared_from_this(), i));
            }
        }
        return list;
    }
    size_t size() const
    {
        return entity_components_.size() - free_list_.size();
    }
private:
    void accomodate_entity(Entity::Id index);
    void accomodate_component(TypeId index);

    uint32_t index_counter_ = 0;

    // std::vector de components type
    // std::vector d'entity
    // // list de components (du bon type ;) )!
    std::vector<std::vector<BaseComponent::List>> entity_components_;
    std::list<Entity::Id> free_list_;
};

template <typename C>
std::shared_ptr<C> EntityManager::assign(const Entity::Id& id, std::shared_ptr<C> component)
{
    std::shared_ptr<BaseComponent> base = std::static_pointer_cast<BaseComponent>(component);
    if(!event_manager_) throw ;
    base->event_manager_ = event_manager_;
    accomodate_component(C::getTypeId());
    entity_components_.at(C::getTypeId()).at(id).push_back(base);
    return component;
}

template <typename C>
std::shared_ptr<C> Entity::assign(std::shared_ptr<C> component)
{
    return manager_.lock()->assign<C>(id_, component);
}

template <typename C>
void Entity::remove(unsigned int index)
{
    manager_.lock()->remove<C>(id_, index);
}

template <typename C>
std::shared_ptr<C> Entity::component(unsigned int index) const
{
    if(valid() == false) return nullptr;
    return manager_.lock()->component<C>(id_, index);
}

template <typename C>
unsigned int Entity::count()
{
    return manager_.lock()->count<C>(id_);
}


}

#endif // ECS_ENTITY_MANAGER_H
