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

#ifndef BEBLOB_MATH_H
#define BEBLOB_MATH_H

namespace beblob
{
namespace math
{

inline double modulo(double x, double y)
{
    x-=y*abs(x/y);
    if (x>=0.) return (x);
    else return (x+y);
}

inline float lerp(float value, float start, float end)
{
    return start + (end - start) * value;
}

template <typename T>
inline T clamp(T value, T vmin, T vmax)
{
    return std::max(vmin, std::min(value, vmax));
}

inline int pnpoly(const std::vector<sf::Vector2f>& points, const sf::Vector2f& test)
{
    uint32_t i, j;
    int c = 0;

    for (i = 0, j = points.size()-1; i < points.size(); j = i++)
    {
        if ( ((points[i].y > test.y) != (points[j].y > test.y)) &&
             (test.x < (points[j].x-points[i].x)
              * (test.y-points[i].y) / (points[j].y-points[i].y) + points[i].x) )
        {
            c = !c;
        }
    }
    return c;
}

inline bool findIntersection(const sf::Vector2f& p1,
                      const sf::Vector2f& p2,
                      const sf::Vector2f& p3,
                      const sf::Vector2f& p4,
                      sf::Vector2f& pt) {
    float xD1,yD1,xD2,yD2,xD3,yD3;
    float dot,deg,len1,len2;
    float segmentLen1,segmentLen2;
    float ua;
    float div;

    // calculate differences
    xD1=p2.x-p1.x;
    xD2=p4.x-p3.x;
    yD1=p2.y-p1.y;
    yD2=p4.y-p3.y;
    xD3=p1.x-p3.x;
    yD3=p1.y-p3.y;

    // calculate the lengths of the two lines
    len1=sqrt(xD1*xD1+yD1*yD1);
    len2=sqrt(xD2*xD2+yD2*yD2);

    // calculate angle between the two lines.
    dot=(xD1*xD2+yD1*yD2); // dot product
    deg=dot/(len1*len2);

    // if abs(angle)==1 then the lines are parallell,
    // so no intersection is possible
    if(abs(deg)==1) return false;

    // find intersection Pt between two lines
    div=yD2*xD1-xD2*yD1;
    ua=(xD2*yD3-yD2*xD3)/div;
    //float ub = (xD1*yD3-yD1*xD3)/div;
    pt.x=p1.x+ua*xD1;
    pt.y=p1.y+ua*yD1;

    // calculate the combined length of the two segments
    // between Pt-p1 and Pt-p2
    xD1=pt.x-p1.x;
    xD2=pt.x-p2.x;
    yD1=pt.y-p1.y;
    yD2=pt.y-p2.y;
    segmentLen1=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

    // calculate the combined length of the two segments
    // between Pt-p3 and Pt-p4
    xD1=pt.x-p3.x;
    xD2=pt.x-p4.x;
    yD1=pt.y-p3.y;
    yD2=pt.y-p4.y;
    segmentLen2=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

    // if the lengths of both sets of segments are the same as
    // the lenghts of the two lines the point is actually
    // on the line segment.

    // if the point isn’t on the line, return null
    if(abs(len1-segmentLen1)>0.01 || abs(len2-segmentLen2)>0.01)
        return false;

    // return the valid intersection
    return true;
}


inline bool testcoll(const std::vector<sf::Vector2f>& points,
              const sf::Vector2f& a,
              const sf::Vector2f& b,
              sf::Vector2f& result)
{
    for(unsigned int i=0;i<points.size();i++)
    {
        if(i == points.size()-1)
        {
            if(findIntersection(points[i],
                                points[0], a, b,
                                result))
                return true;
        }
        else
        {
            if(findIntersection(points[i],
                                points[i+1], a, b,
                                result))
                return true;
        }
    }
    return false;
}

template <typename T>
inline bool out_of_range(T value, T min, T max)
{
    return value < min || value > max;
}

}
}

#endif // BEBLOB_MATH_H
