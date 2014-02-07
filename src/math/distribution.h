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

#ifndef BEBLOB_DISTRIBUTION_H
#define BEBLOB_DISTRIBUTION_H

#include <functional>

namespace beblob
{

namespace math
{

//namespace
//{
template <typename T>
struct Constant
{
    explicit Constant(T value)
        : value(value)
    {
    }

    T operator() () const
    {
        return value;
    }

    T value;
};
//}
#define AURORA_ENABLE_IF(...)  , typename std::enable_if<__VA_ARGS__>::type* = nullptr

template <typename T>
class Distribution
{
private:
    typedef std::function<T()> FactoryFn;

public:
    template <typename U>
                                Distribution(U constant
                                    AURORA_ENABLE_IF(std::is_convertible<U, T>::value))
    : mFactory(Constant<T>(constant))
    {
    }

    /// @brief Construct from distribution function
    /// @param function Callable convertible to std::function<T()>. Every time the operator() of this distribution is invoked,
    ///  it returns the return value of the specified function.
    template <typename Fn>
                                Distribution(Fn function
                                    AURORA_ENABLE_IF(!std::is_convertible<Fn, T>::value))
    : mFactory(function)
    {
    }

    T operator() () const
    {
        return mFactory();
    }

    void swap(Distribution<T>& other)
    {
        mFactory.swap(other.mFactory);
    }


private:
    FactoryFn					mFactory;
};

template <typename T>
void swap(Distribution<T>& lhs, Distribution<T>& rhs)
{
    lhs.swap(rhs);
}

}
}

#endif // BEBLOB_DISTRIBUTION_H
