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

#include "ecs/ecs.h"
#include "systems/graphics.h"
#include "systems/physics.h"
#include "systems/logics.h"
#include "systems/sounds.h"
#include "systems/control.h"

using namespace beblob;

int main()
{
    ecs::Manager manager;

    // renderer
    manager.addSystem<system::Graphics>(0);
    // physic manager
    manager.addSystem<system::Physics>(1);
    // ai
    manager.addSystem<system::Logics>(1);
    // sounds management <boum!>
    manager.addSystem<system::Sounds>(1);
    // keyboard and mouse interaction
    manager.addSystem<system::Control>(2);

    return manager.run();
}
