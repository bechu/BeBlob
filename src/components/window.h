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

#ifndef BEBLOB_COMPONENT_WINDOW_H
#define BEBLOB_COMPONENT_WINDOW_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "ecs/ecs.h"

namespace beblob
{
namespace component
{

////////////////////////////////////////////////////////////
/// \brief abstract class of SFML window
////////////////////////////////////////////////////////////
class Window
        : public ecs::Component<Window>
{
public:
    /// shared pointer definition
    typedef std::shared_ptr<Window> Ptr;

    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ///
    /// \param width window size
    /// \param height window size
    ////////////////////////////////////////////////////////////
    Window(uint32_t width, uint32_t height);

    ////////////////////////////////////////////////////////////
    /// \brief destructor
    ////////////////////////////////////////////////////////////
    ~Window();

    ////////////////////////////////////////////////////////////
    /// \brief set the center of the windows
    ///
    /// \param center window center
    ////////////////////////////////////////////////////////////
    void setCenter(const sf::Vector2f& center=sf::Vector2f());

    ////////////////////////////////////////////////////////////
    /// \brief update the view
    ////////////////////////////////////////////////////////////
    void updateView();

    ////////////////////////////////////////////////////////////
    /// \brief return a shared pointer of the render
    ////////////////////////////////////////////////////////////
    std::shared_ptr<sf::RenderWindow> operator()();

    sf::Vector2f getMousePosition() const;
    void uiView();
    void defaultView();
    sf::Vector2f toMap(const sf::Vector2f& point);
    float getZoom() const;
    sf::View fixed;
private:
    std::shared_ptr<sf::RenderWindow> window_;
    sf::Vector2i mouse_;
    sf::View base;
    float x;
    float y;
    float zoom = 1;
    sf::Clock clock_dbl_click_;
    sf::Vector2f first_click_;
};

inline float Window::getZoom() const
{
    return zoom;
}

inline std::shared_ptr<sf::RenderWindow> Window::operator()()
{
    return window_;
}

}
}

#endif // BEBLOB_COMPONENT_WINDOW_H
