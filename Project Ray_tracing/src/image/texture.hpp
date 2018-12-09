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


#pragma once
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "image.hpp"


namespace cpe
{
struct color;
class vec2;

/** Container for picture/texture data. Texture is accessible as (u,v) coordinates where (u,v) are floating values in [0,1]. */
struct texture : image
{
    using image::image;

    /** Color access. (u,v) must be in [0,1]
     * The color is computed using bi-linear interpolation. */
    color operator()(float u,float v) const;
    /** Color access. (u,v) must be in [0,1]
      * The color is computed using bi-linear interpolation. */
    color operator()(vec2 const& uv) const;


};


}

#endif
