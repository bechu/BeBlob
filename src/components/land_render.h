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

#ifndef BEBLOB_LAND_H
#define BEBLOB_LAND_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "ecs/component.h"
#include "land_physic.h"

namespace beblob
{
namespace component
{

class LandRender
        : public ecs::Component<LandRender>
        , public sf::Drawable
        , public sf::Transformable
{
public:
    /// land render shared pointer
    typedef std::shared_ptr<LandRender> Ptr;

    LandRender();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void compute(component::LandSolid& land);
    static sf::Texture texture;
    bool isInside(const sf::FloatRect& rect);
private:
    sf::VertexArray vertices_;
    sf::Vector2f pos;
    sf::Texture* text;
};

}
}


#endif // BEBLOB_LAND_H
