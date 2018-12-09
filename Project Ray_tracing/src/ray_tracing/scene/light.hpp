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

#ifndef LIGHT_HPP
#define LIGHT_HPP


#include "lib/3d/vec3.hpp"
#include "image/color.hpp"

namespace cpe
{

/** Container of a light structure */
struct light
{
    /** Direct constructor */
    light(vec3 const& position_param=vec3(),
          color const& color_param=color(1.0f,1.0f,1.0f),
          float power_param=1.0f);

    /** Light position */
    vec3 p;
    /** Light color */
    color c;
    /** Light power */
    float power;
};

}

#endif
