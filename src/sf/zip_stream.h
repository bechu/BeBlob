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

#ifndef BEBLOB_ZIP_STREAM_H
#define BEBLOB_ZIP_STREAM_H

#include <string>
#include <vector>
#include <zip.h>

#include <SFML/System/InputStream.hpp>

namespace sf
{

////////////////////////////////////////////////////////////
/// \brief zip stream implementation
////////////////////////////////////////////////////////////
class ZipStream
        : public sf::InputStream
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief constructor
    ///
    /// \param path the zip path (with no password)
    ////////////////////////////////////////////////////////////
    ZipStream(const std::string& path);

    ////////////////////////////////////////////////////////////
    /// \brief desctructor : close the zip handle
    ////////////////////////////////////////////////////////////
    virtual ~ZipStream();

    ////////////////////////////////////////////////////////////
    /// \brief open a particular file in the zip
    ///
    /// \param filename name of the file to use
    ////////////////////////////////////////////////////////////
    bool open(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief read data from the buffer
    ///
    /// \param data container
    /// \param size number of bytes
    ///
    /// \return Int64 the size readed
    ////////////////////////////////////////////////////////////
    sf::Int64 read(void* data, sf::Int64 size);

    ////////////////////////////////////////////////////////////
    /// \brief move the cursor
    ///
    /// \param position new position of the cursor
    ///
    /// \return current cursor position
    ////////////////////////////////////////////////////////////
    sf::Int64 seek(sf::Int64 position);

    ////////////////////////////////////////////////////////////
    /// \brief get the cursor value
    ///
    /// \return current cursor position
    ////////////////////////////////////////////////////////////
    sf::Int64 tell();

    ////////////////////////////////////////////////////////////
    /// \brief get the buffer size
    ///
    /// \return buffer size
    ////////////////////////////////////////////////////////////
    sf::Int64 getSize();
private:
    /// handle of the zip
    zip* handle_ = 0;
    /// current position in the file
    sf::Uint64 cursor_ = 0;
    /// the buffer (contains data of the file opened)
    std::vector<char> buffer_;
};

}

#endif // BEBLOB_ZIP_STREAM_H
