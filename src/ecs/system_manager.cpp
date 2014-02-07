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

#include <algorithm>

#include "system_manager.h"

namespace ecs
{

void SystemManager::update(double dt)
{
    // call the scheduler update
    entity_manager_->update();
    for(auto system : order_) system->update(dt);
}

void SystemManager::initialize()
{
    for(auto system : order_) system->initialize();
}

void SystemManager::configure()
{
    std::sort(order_.begin(), order_.end(), [](const BaseSystem::Ptr lhs, const BaseSystem::Ptr rhs)
    {
      return lhs->getPriority() < rhs->getPriority();
    });
    for(auto system : order_) system->configure();
}

void SystemManager::release()
{
    for(auto system : order_) system->release();
}

}
