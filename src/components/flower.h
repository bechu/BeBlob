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

#ifndef BEBLOB_GRAPHIC_FLOWER_H
#define BEBLOB_GRAPHIC_FLOWER_H


#include <SFML/Graphics.hpp>

#include "math/random.h"
#include "ecs/component.h"
#include "sf/curve.h"

namespace beblob
{
namespace component
{

class Flower
        : public ecs::Component<Flower>
        , public sf::Drawable
        , public sf::Transformable
{
public:
    struct Draw
    {
        std::vector<sf::Vertex> outside;
        std::vector<sf::Vertex> inside;
    };
    /// maximum number of state for a flower
    constexpr static int kMaxState = 10;

    Flower();
    void compute(int q);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    static void prepare(math::Random::Seed seed);

    ////////////////////////////////////////////////////////////
    /// \brief create a flower texture for a particular state
    ///
    /// \param state the state of the flower [0-9]
    ////////////////////////////////////////////////////////////
    static void createTexture(int index,
                              math::Random::Seed seed,
                              Flower::Draw &draw);
    static std::shared_ptr<sf::RenderTexture>
    drawTexture(const Flower::Draw& draw);

    static void clear();
private:
    struct Petal
    {
        Petal(float width, float height);
        std::vector<sf::Vector2f> get(unsigned int width,
                                      unsigned int height,
                                      unsigned int rotation);
        std::vector<sf::Vector2f> points_;
    };
    sf::Curve stem_;
    std::vector<sf::VertexArray> shapes_;
    int last_q;

    std::weak_ptr<sf::RenderTexture> current_;

    /// size of the flower in pixel (square)
    constexpr static int kSize = 60;

    /// flower textures zone
    static std::vector<std::shared_ptr<sf::RenderTexture>> textures_;

    static math::Random rng_;
    static std::vector<sf::Color> Colors;
    static int w;
    static int h;
    static int w2;
    static int h2;
    static std::vector<float> f;
};

}
}

#endif // BEBLOB_GRAPHIC_FLOWER_H
