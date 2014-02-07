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

#ifndef BEBLOB_GRAPHICS_BLOB_H
#define BEBLOB_GRAPHICS_BLOB_H

#include <SFML/Graphics.hpp>

#include "blob_solid.h"
#include "blob_ai.h"
#include "math/random.h"
#include <SFML/Graphics.hpp>

#include "ecs/component.h"
#include "sf/curve.h"

namespace beblob
{
namespace component
{

////////////////////////////////////////////////////////////
/// \brief graphic blob definition
////////////////////////////////////////////////////////////
class BlobRender
        : public ecs::Component<BlobRender>
        , public sf::Drawable
        , public sf::Transformable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief blob render constructor
    ////////////////////////////////////////////////////////////
    BlobRender();

    ////////////////////////////////////////////////////////////
    /// \brief compute the render
    ///
    /// \param dt diff time
    /// \param physic blob solid
    /// \param mouse where the mouse (for the eye tracker)
    ////////////////////////////////////////////////////////////
    void compute(double dt,
                 BlobSolid& physic,
                 BlobAI &logic,
                 const sf::Vector2f& mouse);

    ////////////////////////////////////////////////////////////
    /// \brief set the color of the blob
    ///
    /// \param color
    ////////////////////////////////////////////////////////////
    void setFillColor(const sf::Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief return the color of the blob
    ///
    /// \return color
    ////////////////////////////////////////////////////////////
    sf::Color getFillColor() const;

    ////////////////////////////////////////////////////////////
    /// \brief draw the blob
    ////////////////////////////////////////////////////////////
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    ////////////////////////////////////////////////////////////
    /// \brief this is the arc shape definition
    ////////////////////////////////////////////////////////////
    class Arc : public sf::Drawable, public sf::Transformable
    {
    public:
        void compute(const sf::Vector2f &p,
                     float aRadius,
                     float startDeg,
                     float numDegs,
                     bool filled = true,
                     const sf::Color &color=sf::Color());
        void clear();
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    protected:
        /// list of vertex
        std::vector<sf::VertexArray> vertex_;
    };

    ////////////////////////////////////////////////////////////
    /// \brief compute the shape render
    ////////////////////////////////////////////////////////////
    void computeShape(BlobSolid& physic, BlobAI &logic);

    ////////////////////////////////////////////////////////////
    /// \brief compute the face render
    ////////////////////////////////////////////////////////////
    void computeFace(double dt,
                     BlobSolid& physic,
                     BlobAI& logic,
                     const sf::Vector2f &mouse);

    struct Mouth : public Arc
    {
        void update(double dt, BlobSolid& physic, float r);
    };

    struct Eye : public Arc
    {
        void update(BlobSolid& physic, float offset, const sf::Vector2f &mouse,
                    float r);
        void open  (BlobSolid& physic, float offset, const sf::Vector2f &mouse);
        void blink (BlobSolid& physic, float offset, const sf::Vector2f &mouse);
        void ooh   (BlobSolid& physic, float offset, const sf::Vector2f &mouse);
        sf::Curve curve_;
    };

    /// global shape
    sf::VertexArray shape_;
    /// curve around shape
    sf::Curve around_shape_;
    /// mouth
    Mouth mouth_;
    /// left eye
    Eye left_;
    /// right eye
    Eye right_;
    /// around the shape
    sf::Curve ring_;
    /// the current state of the blob
    sf::Curve state_;
    /// the color of the blob
    sf::Color fillColor_;
    /// the lask known position of the mouse
    sf::Vector2f last_mouse_position_;
    /// a local random for blink/open/close eyes
    math::Random rng;
    /// local random storage value
    float r;
    /// idem, kind of timeout
    double change = 1000;
    /// for the rng initialisation
    static int count;
};

inline sf::Color BlobRender::getFillColor() const
{
    return fillColor_;
}

inline void BlobRender::setFillColor(const sf::Color& color)
{
    fillColor_ = color;
}

inline void BlobRender::Arc::clear()
{
    vertex_.clear();
}

}
}

#endif // BEBLOB_GRAPHICS_BLOB_H
