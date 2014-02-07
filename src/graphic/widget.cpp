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

#include "widget.h"

namespace beblob
{
namespace component
{

Widget::Widget(ecs::EventManager::Ptr events) : events_(events)
{
    static sf::ZipStream zip = {"blob.pack"};
    zip.open("font.ttf");
    font_.loadFromStream(zip);
}

void Widget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for(auto model : model_)
        target.draw(*model, states);
}

void Widget::rectangle(const sf::Color& color, bool fill, const sf::Texture& texture)
{
    std::shared_ptr<sf::RectangleShape> rr = std::make_shared<sf::RectangleShape>(size_);
    if(fill)
    {
        rr->setFillColor(color);
    }
    else
    {
        rr->setFillColor(sf::Color(0, 0, 0, 0));
        rr->setOutlineColor(color);
        rr->setOutlineThickness(2);
    }
    if(texture.getSize().x > 0)
        rr->setTexture(&texture);
    rr->setOrigin(sf::Vector2f(size_.x/2, size_.y/2));
    model_.push_back(rr);
}

void Widget::circle(float radius, const sf::Color& color, bool fill, bool center)
{
    std::shared_ptr<sf::CircleShape> rr = std::make_shared<sf::CircleShape>(radius);
    if(fill)
        rr->setFillColor(color);
    else
    {
        rr->setFillColor(sf::Color(0, 0, 0, 0));
        rr->setOutlineColor(color);
        rr->setOutlineThickness(2);
    }
    if(center)
        rr->setOrigin(sf::Vector2f(radius, radius));
    model_.push_back(rr);
}

void Widget::image(const std::string& path, const sf::Vector2f& position, const sf::Color& color)
{
    static std::map<std::string, sf::Texture> textures;
    if(textures.find(path) == textures.end())
    {
        textures[path] = sf::Texture();
        textures[path].loadFromFile(path);
    }
    std::shared_ptr<sf::Sprite> s = std::make_shared<sf::Sprite>(textures[path]);
    s->setPosition(position);
    s->setColor(color);
    model_.push_back(s);
}

void Widget::text(const sf::Vector2f& position,
                  const std::wstring& words,
                  const sf::Color& color,
                  uint32_t font_size)
{

    //std::shared_ptr<sf::Text> text = std::make_shared<sf::Text>(sf::String(words), font);
    std::shared_ptr<sf::Text> text = std::make_shared<sf::Text>();
    static sf::String test;

    test = words;

    if(!text) throw;
    text->setFont(font_);
    text->setString(test);
    text->setCharacterSize(font_size);
    text->setColor(color);
    text->setPosition(position);
    text->setOrigin(sf::Vector2f(size_.x/2, size_.y/2));
    model_.push_back(text);
}

void Widget::clear()
{
    for(auto &model : model_)
        model.reset();
    model_.clear();
}

void Widget::update(double dt)
{
}

}
}

