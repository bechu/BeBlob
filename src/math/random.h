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

#ifndef BEBLOB_RANDOM_H
#define BEBLOB_RANDOM_H

#include <random>
#include <array>
#include <algorithm>

namespace beblob
{
namespace math
{

////////////////////////////////////////////////////////////
/// \brief random constructor
////////////////////////////////////////////////////////////
struct Random
{
    /// the seed definition
    typedef uint64_t Seed;
    ////////////////////////////////////////////////////////////
    /// \brief random constructor
    ////////////////////////////////////////////////////////////
    Random();

    ////////////////////////////////////////////////////////////
    /// \brief set the seed of the random generator
    ///
    /// \param seed new seed
    ////////////////////////////////////////////////////////////
    void setSeed(Seed seed);

    ////////////////////////////////////////////////////////////
    /// \brief return the next unsigned char random value
    ///
    /// \return a random unsigned char
    ////////////////////////////////////////////////////////////
    unsigned char nextByte();
    int next(int m);
    float nextFloat();
    int rangeInt(int min, int max);
    float range(float min, float max);
    double nextDouble();
private:
    std::mt19937 rd;
};

}
}

#endif // BEBLOB_RANDOM_H
