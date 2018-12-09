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

#include "image_zbuffer.hpp"
#include "io/image_ppm.hpp"
#include "lib/common/basic_functions.hpp"

namespace cpe
{

image_zbuffer::image_zbuffer()
    :image_basic<float>()
{}

image_zbuffer::image_zbuffer(int const N)
    :image_basic<float>(N)
{
    fill(1.0f);
}

image_zbuffer::image_zbuffer(int const Nx,int const Ny)
    :image_basic<float>(Nx,Ny)
{
    fill(1.0f);
}


void image_zbuffer::save(std::string const& filename) const
{
    if( extension(filename) == ".ppm" )
        export_ppm(filename,*this);
    else
        throw cpe::exception_cpe("Unknown format for saving file "+filename,EXCEPTION_PARAMETERS_CPE);
}

}
