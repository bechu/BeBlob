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

#include "random.h"

namespace beblob
{
namespace math
{

Random::Random()
{
    std::array<int, std::mt19937::state_size>seed_data;
    std::random_device r;
    std::generate_n(seed_data.data(),seed_data.size(), std::ref(r));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    rd.seed(seq);
}

void Random::setSeed(math::Random::Seed seed)
{
    rd.seed(seed);
}

unsigned char Random::nextByte()
{
    std::uniform_int_distribution<int> dis(0, 255);
    return static_cast<unsigned char>(dis(rd));
}

int Random::next(int m)
{
    std::uniform_int_distribution<int> dis(0, m);
    return dis(rd);
}

float Random::nextFloat()
{
    std::uniform_real_distribution<> dis(0, 1);
    return dis(rd);
}

int Random::rangeInt(int min, int max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(rd);
}

float Random::range(float min, float max)
{
    std::uniform_real_distribution<> dis(min, max);
    return dis(rd);
}

double Random::nextDouble()
{
    std::uniform_real_distribution<> dis(0, 1);
    return dis(rd);
}

}
}
