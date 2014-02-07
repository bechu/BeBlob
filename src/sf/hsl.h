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

#ifndef BEBLOB_HSL_H
#define BEBLOB_HSL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf
{

////////////////////////////////////////////////////////////
/// \brief hsl sf manipulation
////////////////////////////////////////////////////////////
struct HSL
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief hsl constructor
    ///
    /// \param c the origin color
    ////////////////////////////////////////////////////////////
    HSL(const sf::Color& c) :
        h(0),
        l(0),
        s(0)
    {
        *this = c;
    }

    ////////////////////////////////////////////////////////////
    /// \brief hsl constructor
    ///
    /// \param hue
    /// \param saturation
    /// \param lighness
    ////////////////////////////////////////////////////////////
    HSL(double hue=0, double saturation=0, double lightness = 0)
        : h(hue),
          l(lightness),
          s(saturation)
    {
    }

    ////////////////////////////////////////////////////////////
    /// \brief return the color from instance
    ///
    /// \return color generated from instance
    ////////////////////////////////////////////////////////////
    sf::Color operator()()
    {
        double t[] = { 0, 0, 0 };

        double tH = h;
        double tS = s;
        double tL = l;

        if (tS == 0)
        {
            t[0] = t[1] = t[2] = tL;
        }
        else
        {
            double q, p;

            q = tL < 0.5 ? tL * (1 + tS) : tL + tS - (tL * tS);
            p = 2 * tL - q;

            t[0] = tH + (1.0 / 3.0);
            t[1] = tH;
            t[2] = tH - (1.0 / 3.0);

            for (int i = 0; i < 3; i++)
            {
                t[i] = t[i] < 0 ? t[i] + 1.0 : t[i] > 1 ? t[i] - 1.0 : t[i];

                if (t[i] * 6.0 < 1.0)
                    t[i] = p + ((q - p) * 6 * t[i]);
                else
                    if (t[i] * 2.0 < 1.0)
                        t[i] = q;
                    else
                        if (t[i] * 3.0 < 2.0)
                            t[i] = p + ((q - p) * 6 * ((2.0 / 3.0) - t[i]));
                        else
                            t[i] = p;
            }
        }

        return sf::Color((int)(t[0] * 255), (int)(t[1] * 255), (int)(t[2] * 255));
    }

    ////////////////////////////////////////////////////////////
    /// \brief set the color
    ////////////////////////////////////////////////////////////
    HSL& operator=(const sf::Color &c)
    {
        double r = c.r / 255.0;
        double g = c.g / 255.0;
        double b = c.b / 255.0;
        double max = std::max(std::max(r, g), b);
        double min = std::min(std::min(r, g), b);
        h = s = l = (max + min) / 2;

        if(max == min){
            h = s = 0; // achromatic
        }else{
            double d = max - min;
            s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
            if(max == r) h = (g - b) / d + (g < b ? 6 : 0);
            else if(max == g) h = (b - r) / d + 2;
            else if(max == b) h = (r - g) / d + 4;
        }
        h /= 6;
        return *this;
    }

    double h;
    double l;
    double s;
};

}

#endif // BEBLOB_HSL_H

