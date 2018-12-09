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

#include "texture.hpp"
#include "color.hpp"
#include "lib/3d/vec2.hpp"

namespace cpe
{

color texture::operator()(float const u,float const v) const
{

    ASSERT_CPE(u>=0.0f && u<=1.0f && v>=0.0f && v<=1.0f , "(u,v) must be in [0,1]");

    //convert (u,v) in pixel value.
    // v is inverted due to the vertical system axis.
    float const u_pixel = u*(Nx()-2);
    float const v_pixel = (1-v)*(Ny()-2);

    //sanity check
    ASSERT_CPE(u_pixel>=0 && u_pixel<Nx(),"error u_pixel");
    ASSERT_CPE(v_pixel>=0 && v_pixel<Ny(),"error u_pixel");

    //get neighboring indices
    int const u_k0 = static_cast<int>(u_pixel);
    int const v_k0 = static_cast<int>(v_pixel);

    int const u_k1 = u_k0+1;
    int const v_k1 = v_k0+1;

    float const u_alpha = u_pixel-u_k0;
    float const v_alpha = v_pixel-v_k0;

    int const N = Nx();

    //sanity check
    ASSERT_CPE(u_k0>=0 && u_k0<Nx() , "error too large u_k0");
    ASSERT_CPE(u_k1>=0 && u_k1<Nx() , "error too large u_k1");
    ASSERT_CPE(v_k0>=0 && v_k0<Ny() , "error too large v_k0");
    ASSERT_CPE(v_k1>=0 && v_k1<Ny() , "error too large v_k1");

    //get the 4 neighboring colors
    color const& c00 = (*this)[u_k0+N*v_k0];
    color const& c01 = (*this)[u_k0+N*v_k1];
    color const& c10 = (*this)[u_k1+N*v_k0];
    color const& c11 = (*this)[u_k1+N*v_k1];

    //bi-linear interpolation
    color const interpolated = (1.0f-u_alpha)*(1.0f-v_alpha)*c00+
            (1.0f-u_alpha)*v_alpha*c01+
            u_alpha*(1.0f-v_alpha)*c10+
            u_alpha*v_alpha*c11;

    return interpolated;

}

color texture::operator()(vec2 const& uv) const
{
    return (*this)(uv.x(),uv.y());
}

}
