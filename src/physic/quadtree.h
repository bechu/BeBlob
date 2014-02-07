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

#ifndef BEBLOB_QUADTREE_H
#define BEBLOB_QUADTREE_H

#include <vector>
#include <memory>

#include "ecs/component.h"

namespace beblob
{
namespace physic
{

////////////////////////////////////////////////////////////
/// \brief QuadTree class
////////////////////////////////////////////////////////////
class QuadTree
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief QuadTree object
    ////////////////////////////////////////////////////////////
    struct Object : public ecs::Component<Object>
    {
        /// pointer
        typedef std::shared_ptr<Object> Ptr;
        /// list of pointers
        typedef std::vector<Ptr> List;

        ////////////////////////////////////////////////////////////
        /// \brief Object creation
        ///
        /// \param x x position
        /// \param y y position
        /// \param WIDTH object's width
        /// \param HEIGHT object's height
        ////////////////////////////////////////////////////////////
        Object(float X, float Y, float WIDTH, float HEIGHT);

        /// x position
        float x;
        /// y position
        float y;
        /// width of the position
        float width;
        /// height of the position
        float height;
    };

    ////////////////////////////////////////////////////////////
    /// \brief Object creation
    ///
    /// \param x
    /// \param y
    /// \param
    /// \param
    /// \param level
    ////////////////////////////////////////////////////////////
    QuadTree(float x, float y, float width, float height, int level=0);

    ////////////////////////////////////////////////////////////
    /// \brief destructor
    ////////////////////////////////////////////////////////////
    ~QuadTree();

    ////////////////////////////////////////////////////////////
    /// \brief add an object
    ///
    /// \param item object
    ////////////////////////////////////////////////////////////
    void add(Object::Ptr item);

    ////////////////////////////////////////////////////////////
    /// \brief remove an object
    ///
    /// \param item object
    ////////////////////////////////////////////////////////////
    void remove(Object::Ptr item);

    ////////////////////////////////////////////////////////////
    /// \brief return a list of object
    ///
    /// \param x x position
    /// \param y y position
    ///
    /// \return list of objects
    ////////////////////////////////////////////////////////////
    Object::List get(float x, float y);
private:
    /// the maximum level of the quadtree
    constexpr static const int kMaxLevel = 4;

    ////////////////////////////////////////////////////////////
    /// \brief remove internal object
    ///
    /// \param item object
    ////////////////////////////////////////////////////////////
    void remove_object(Object::Ptr item);

    ////////////////////////////////////////////////////////////
    /// \brief test if an object is in a quadtree
    ///
    /// \param child sub quadtree
    /// \param object
    ///
    /// \return true if the object is in the quadtree
    ////////////////////////////////////////////////////////////
    bool contains(QuadTree* child, Object::Ptr object);

    /// top left tree
    QuadTree* TopLeft = 0;
    /// top right tree
    QuadTree* TopRight = 0;
    /// bottom left tree
    QuadTree* BottomLeft = 0;
    /// bottom right tree
    QuadTree* BottomRight = 0;
    /// objects list
    Object::List objects_;
    /// the root object
    Object _;
    /// level of the quadtree
    int level_ = 0;
};

}
}

#endif // BEBLOB_QUADTREE_H
