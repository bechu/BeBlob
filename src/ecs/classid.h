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

#ifndef BEBLOB_ECS_CLASSID_H
#define BEBLOB_ECS_CLASSID_H

#include <cstddef>

namespace ecs
{

typedef std::size_t TypeId;

template <typename TBase>
class ClassTypeId
{
public:

    template <typename T>
    static TypeId getTypeId()
    {
        static const TypeId id = getNextTypeId();
        return id;
    }

private:

    static TypeId getNextTypeId()
    {
        return nextTypeId_++;
    }

    static TypeId nextTypeId_;
};

template <typename TBase>
TypeId ClassTypeId<TBase>::nextTypeId_ = 0;

}

#endif // BEBLOB_ECS_CLASSID_H
