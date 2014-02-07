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

#include "entity_manager.h"

namespace ecs
{

EntityManager::EntityManager()
{
}

Entity EntityManager::createEntity()
{
    Entity::Id index;
    if(free_list_.empty())
    {
        index = index_counter_++;
    }
    else
    {
        index = free_list_.front();
        free_list_.pop_front();
    }
    accomodate_entity(index);
    return Entity(shared_from_this(), Entity::Id(index));
}

void EntityManager::destroy(Entity::Id id)
{
    free_list_.push_back(id);
    for(auto &components_type : entity_components_)
    {
        if(id < components_type.size())
        {
            auto &components = components_type[id];
            for(auto component : components)
            {
                component.reset();
            }
            components.clear();
        }
    }
}


void EntityManager::accomodate_component(TypeId index)
{
    if(entity_components_.size() <= index)
    {
        entity_components_.resize(index+1);
        accomodate_entity(index_counter_);
    }
}
void EntityManager::accomodate_entity(Entity::Id index)
{
    for (auto &components : entity_components_)
    {
        if(components.size() <= index)
            components.resize(index+1);
    }
}


}
