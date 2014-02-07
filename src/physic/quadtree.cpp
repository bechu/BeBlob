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

#include <limits>

#include "quadtree.h"

namespace beblob
{
namespace physic
{

QuadTree::QuadTree(float x, float y, float width, float height, int level)
    : _(x, y, width, height), level_(level)
{
    if(level_ == kMaxLevel) return ;
    TopLeft = new QuadTree(x, y, width/2.0f, height/2.0f, level_+1);
    TopRight = new QuadTree(x + width / 2.0f, y, width / 2.0f,
                            height / 2.0f, level+1);
    BottomLeft = new QuadTree(x, y + height / 2.0f, width / 2.0f,
                              height / 2.0f, level+1);
    BottomRight = new QuadTree(x + width / 2.0f, y + height / 2.0f,
                               width / 2.0f, height / 2.0f, level+1);
}

QuadTree::~QuadTree()
{
    if(TopLeft) delete TopLeft;
    if(TopRight) delete TopRight;
    if(BottomLeft) delete BottomLeft;
    if(BottomRight) delete BottomRight;
}

void QuadTree::remove_object(Object::Ptr item)
{
    for(auto it=objects_.begin();it!=objects_.end();++it)
    {
        if(*it == item)
        {
            objects_.erase(it);
            return ;
        }
    }
}

void QuadTree::remove(Object::Ptr object)
{
    if (level_ != kMaxLevel)
    {
        if (contains(TopLeft, object))
        {
            TopLeft->remove(object);
        }
        else if (contains(TopRight, object))
        {
            TopRight->remove(object);
        }
        else if (contains(BottomLeft, object))
        {
            BottomLeft->remove(object);
        }
        else if (contains(BottomRight, object))
        {
            BottomRight->remove(object);
        }
    }
    if (contains(this, object))
        remove_object(object);
}

void QuadTree::add(Object::Ptr object)
{
    if (level_ == kMaxLevel)
    {
        objects_.push_back(object);
        return;
    }

    if (contains(TopLeft, object))
    {
        TopLeft->add(object);
    }
    else if (contains(TopRight, object))
    {
        TopRight->add(object);
    }
    else if (contains(BottomLeft, object))
    {
        BottomLeft->add(object);
    }
    else if (contains(BottomRight, object))
    {
        BottomRight->add(object);
    }
    else if (contains(this, object))
        objects_.push_back(object);
}

QuadTree::Object::List QuadTree::get(float x, float y)
{
    if (level_ == kMaxLevel)
        return objects_;

    Object::List returnObjects, childReturnObjects;

    if (!objects_.empty())
        returnObjects = objects_;

    if (x > _.x + _.width / 2.0f && x < _.x + _.width)
    {
        if (y > _.y + _.height / 2.0f && y < _.y + _.height)
        {
            childReturnObjects = BottomRight->get(x, y);
            returnObjects.insert(returnObjects.end(),
                                 childReturnObjects.begin(),
                                 childReturnObjects.end());
            return returnObjects;
        }
        else if (y > _.y && y <= _.y + _.height / 2.0f)
        {
            childReturnObjects = TopRight->get(x, y);
            returnObjects.insert(returnObjects.end(),
                                 childReturnObjects.begin(),
                                 childReturnObjects.end());
            return returnObjects;
        }
    }
    else if (x > _.x && x <= _.x + _.width / 2.0f)
    {
        if (y > _.y + _.height / 2.0f && y < _.y + _.height)
        {
            childReturnObjects = BottomLeft->get(x, y);
            returnObjects.insert(returnObjects.end(),
                                 childReturnObjects.begin(),
                                 childReturnObjects.end());
            return returnObjects;
        }
        else if (y > _.y && y <= _.y + _.height / 2.0f)
        {
            childReturnObjects = TopLeft->get(x, y);
            returnObjects.insert(returnObjects.end(),
                                 childReturnObjects.begin(),
                                 childReturnObjects.end());
            return returnObjects;
        }
    }
    return returnObjects;
}

QuadTree::Object::Object(float X, float Y, float WIDTH, float HEIGHT) : x(X), y(Y), width(WIDTH), height(HEIGHT)
{
}

bool QuadTree::contains(QuadTree* child, Object::Ptr object)
{
    return	 !(object->x < child->_.x ||
               object->y < child->_.y ||
               object->x > child->_.x + child->_.width  ||
               object->y > child->_.y + child->_.height ||
               object->x + object->width < child->_.x ||
               object->y + object->height < child->_.y ||
               object->x + object->width > child->_.x + child->_.width ||
               object->y + object->height > child->_.y + child->_.height);
}

}
}

