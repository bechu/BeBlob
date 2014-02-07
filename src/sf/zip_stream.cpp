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

#include <cstring>
#include <cassert>
#include <iostream>

#include "zip_stream.h"

namespace sf
{

ZipStream::ZipStream(const std::string& path)
{
    int error = 0;
    handle_ = zip_open(path.c_str(), 0, &error);
    if(error != ZIP_ER_OK)
        handle_ = 0;
}

bool ZipStream::open(const std::string& filename)
{
    if(!handle_) return false;
    cursor_ = 0;
    buffer_.clear();
    int index = zip_name_locate(handle_,
        filename.c_str(),
        0);
        //ZIP_FL_ENC_GUESS | ZIP_FL_NOCASE | ZIP_FL_UNCHANGED);
    cursor_ = 0;
    if(index >= 0)
    {
        struct zip_stat stat;
        zip_stat_init(&stat);
        int result = zip_stat_index(handle_, index, ZIP_FL_UNCHANGED, &stat);
        if(result == 0)
        {
            struct zip_file* zipFile = zip_fopen_index(handle_, index, ZIP_FL_UNCHANGED);
            if(zipFile)
            {
                buffer_.resize(stat.size);
                result = zip_fread(zipFile, &buffer_[0], stat.size);
                zip_fclose(zipFile);
                if(result == static_cast<int>(stat.size)) return true;
                buffer_.clear();
                return true;
            }
            else std::cerr<<"zipFile failed"<<std::endl;
        }
        else
            std::cerr<<"zip stat index error"<<std::endl;
    }
    else
        std::cerr<<filename<<" not found !"<<std::endl;
    return false;
}

ZipStream::~ZipStream()
{
    zip_close(handle_);
    handle_ = 0;
}

sf::Int64 ZipStream::read(void* data, sf::Int64 size)
{
    assert(handle_);
    std::memcpy(data, &buffer_[cursor_], size);
    cursor_ += size;
    return size;
}

sf::Int64 ZipStream::seek(sf::Int64 position)
{
    assert(handle_);
    assert(cursor_ <= buffer_.size());
    cursor_ = position;
    return cursor_;
}

sf::Int64 ZipStream::tell()
{
    assert(handle_);
    return cursor_;
}

sf::Int64 ZipStream::getSize()
{
    assert(handle_);
    return buffer_.size();
}

}
