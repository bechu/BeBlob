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

#ifndef BEBLOB_WIDGET_H
#define BEBLOB_WIDGET_H

#include <SFML/Graphics.hpp>
#include "components/flower.h"
#include "sf/zip_stream.h"
#include "ecs/ecs.h"

namespace beblob
{
namespace component
{

class Widget
        : public ecs::Component<Widget>
        , public sf::Drawable
        , public sf::Transformable
{
public:
    Widget(ecs::EventManager::Ptr events);
    void rectangle(const sf::Color& color,
                   bool fill=true,
                   const sf::Texture &texture=sf::Texture());
    void circle(float radius,
                const sf::Color& color,
                bool fill=true,
                bool center=true);
    void text(const sf::Vector2f& position,
              const std::wstring &words,
              const sf::Color& color=sf::Color(),
              uint32_t font_size=24);
    void image(const std::string& path,
               const sf::Vector2f &position,
               const sf::Color &color=sf::Color());

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void clear();
    std::string test;
    void setSize(const sf::Vector2f& size);
    sf::Vector2f getSize() const;
    bool contains(const sf::Vector2f& point) const;

    virtual void onClick(const sf::Vector2f& ui, const sf::Vector2f& map) {}
    virtual void onUnicode(sf::Uint32 unicode) {}
    virtual void onKey(sf::Keyboard::Key key) {}
    virtual void onMouseOut() {}

    ////////////////////////////////////////////////////////////
    /// \brief update the widget
    ///
    /// \param dt diff time
    ////////////////////////////////////////////////////////////
    virtual void update(double dt);

    ecs::EventManager::Ptr events_;

private:
    std::vector<std::shared_ptr<sf::Drawable>> model_;
    sf::Vector2f size_;
    sf::Font font_;
};


inline bool Widget::contains(const sf::Vector2f& point) const
{
    return sf::Rect<float>(getPosition()-sf::Vector2f(size_.x/2, size_.y/2),
                           getSize()).contains(point);
}

inline void Widget::setSize(const sf::Vector2f& size)
{
    size_ = size;
}

inline sf::Vector2f Widget::getSize() const
{
    return size_;
}

}
}

#endif // BEBLOB_WIDGET_H
