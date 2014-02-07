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

#ifndef MLC_ENTITY_H
#define MLC_ENTITY_H

#include <memory>
#include <vector>

#include <unordered_set>

#include "component.h"

namespace ecs
{

class EntityManager;

class Entity
        : std::enable_shared_from_this<Entity>
{
public:
    static const uint64_t kInvalid = -1;

    typedef uint64_t Id;

    typedef std::vector<Entity> List;
    typedef std::shared_ptr<Entity> Ptr;

    bool operator ! () const {
        return !valid();
    }

    Entity();
    Entity(const Entity &) = default;
    Entity(Entity &&) = default;
    Entity &operator = (const Entity &) = default;

    bool operator ==(const Entity &other) const { return id() == other.id(); }
    void destroy();
    bool valid() const;
    void invalidate();

    // assign component
    template <typename C>
    std::shared_ptr<C> assign(std::shared_ptr<C> component);

    template <typename C, typename ... Args>
    std::shared_ptr<C> assign(Args && ... args);

    // get a component
    template <typename C>
    std::shared_ptr<C> component(unsigned int index=0) const;

    template <typename C>
    bool has()
    {
        return static_cast<bool>(component<C>());
    }

    template <typename C>
    unsigned int count();

    // remove a component
    template <typename C>
    void remove(unsigned int index);

    Entity(std::shared_ptr<EntityManager> manager, const Id& id) : manager_(manager), id_(id) {}
    Id id() const
    {
        return id_;
    }

private:
    friend class EntityManager;

    std::weak_ptr<EntityManager> manager_;
    Id id_ = kInvalid;
};



template <typename C, typename ... Args>
std::shared_ptr<C> Entity::assign(Args && ... args)
{
    return assign<C>(std::make_shared<C>(std::forward<Args>(args) ...));
}

}

#endif // MCL_ENTITY_H

