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



#include "image.hpp"
#include "io/image_ppm.hpp"
#include "lib/common/basic_functions.hpp"

namespace cpe
{



void image::save(std::string const& filename) const
{
    if( extension(filename) == ".ppm" )
        export_ppm(filename,*this);
    else
        throw cpe::exception_cpe("Unknown format for saving file "+filename,EXCEPTION_PARAMETERS_CPE);
}

void image::load(std::string const& filename)
{
    if( extension(filename) == ".ppm" )
        *this = read_ppm(filename);
    else
        throw cpe::exception_cpe("Unknown format for loading file "+filename,EXCEPTION_PARAMETERS_CPE);
}

}
