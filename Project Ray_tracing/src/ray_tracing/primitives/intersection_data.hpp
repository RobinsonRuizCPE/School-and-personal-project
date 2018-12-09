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

#ifndef INTERSECTION_DATA_HPP
#define INTERSECTION_DATA_HPP

#include "lib/3d/vec3.hpp"
#include <ostream>

namespace cpe
{
struct intersection_data
{
    intersection_data();
    intersection_data(vec3 const& position_param,
                      vec3 const& normal_param,
                      float relative_param);

    /** Set all the fields of the intersection value */
    void set(vec3 const& position_param,
             vec3 const& normal_param,
             float relative_param);
    /** Set all the fields of the intersection value */
    void set(intersection_data const& intersection);


    /** 3D position of the intersection */
    vec3 position;
    /** Normal of the shape at the intersection point */
    vec3 normal;
    /** Relative position of the interesection along the ray
      * position = ray.p0 + relative*ray.u */
    float relative;
};

std::ostream& operator<<(std::ostream& stream,intersection_data const& intersection);

}

#endif
