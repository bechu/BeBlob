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

#ifndef BEBLOB_MATH_RANGE_H
#define BEBLOB_MATH_RANGE_H

namespace beblob
{
namespace math
{

////////////////////////////////////////////////////////////
/// \brief range, a c++11 feature to iterate on number
////////////////////////////////////////////////////////////
class Range
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief range iterator
    ////////////////////////////////////////////////////////////
    class iterator
    {
        friend class Range;
    public:
        long int operator *() const { return i_; }
        const iterator &operator ++() { ++i_; return *this; }
        iterator operator ++(int) { iterator copy(*this); ++i_; return copy; }

        bool operator ==(const iterator &other) const { return i_ == other.i_; }
        bool operator !=(const iterator &other) const { return i_ != other.i_; }

    protected:
        iterator(long int start) : i_ (start) { }

    private:
        unsigned long i_;
    };

    iterator begin() const { return begin_; }
    iterator end() const { return end_; }
    Range(long int  begin, long int end) : begin_(begin), end_(end) {}
private:
    iterator begin_;
    iterator end_;
};

}
}

#endif // BEBLOB_MATH_RANGE_H
