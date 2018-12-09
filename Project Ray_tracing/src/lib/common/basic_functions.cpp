/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "basic_functions.hpp"

namespace cpe
{

float clamp(float const value)
{
    if(value>1.0f)
        return 1.0f;
    else if(value<0.0f)
        return 0.0f;
    else
        return value;
}

std::string extension(std::string const& filename)
{
    int const N = filename.size();

    int const offset = filename.find_last_of(".");
    if(N<4)
        return "";
    else
        return filename.substr(offset,N-1);
}

}
