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

#include <cassert>

#include "team.h"
#include "components/blob_ai.h"

namespace beblob
{
namespace component
{

Team::Id Team::nextId_ = 0;

Team::Team()
{
    id_ = getNextId();
}

void Team::reset()
{
    nextId_ = 0;
}

void Team::addMember(const ecs::Entity& blob)
{
    members_.push_back(blob);
}

void Team::removeMember(const ecs::Entity& blob)
{
    for(auto it = members_.begin();it!=members_.end();++it)
    {
        if(*it == blob)
        {
            members_.erase(it);
            return ;
        }
    }
}

ecs::Entity& Team::getMember(uint32_t index)
{
    assert(index < members_.size());
    return members_[index];
}

}
}
