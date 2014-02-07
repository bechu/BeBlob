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

#ifndef ECS_COMPONENET_H
#define ECS_COMPONENET_H

#include <iostream>
#include <memory>
#include <vector>
#include <cstdint>

#include "event.h"
#include "classid.h"

namespace ecs
{

class BaseComponent
        : public Emitter
{
public:
    typedef std::shared_ptr<BaseComponent> Ptr;
    typedef std::vector<Ptr> List;
    virtual ~BaseComponent() {}
};

template <typename Derived>
class Component
        : public BaseComponent
{
public:
    static TypeId getTypeId()
    {
        return ClassTypeId<BaseComponent>::getTypeId<Derived>();
    }
private:
};

}

#endif // ECS_COMPONENET_H
