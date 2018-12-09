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

#ifndef RAY_HPP
#define RAY_HPP

#include "lib/3d/vec3.hpp"

namespace cpe
{


/** Ray (initial position + direction) used for ray-tracing algorithm (can be seen as a half line) */
class ray
{
public:

    ray();
    ray(vec3 const& p0_param,vec3 const& u_param);

    /** Starting point */
    vec3 const& p0() const;
    /** Direction */
    vec3 const& u() const;


    /** Evaluate 3D position at x0+t*u */
    vec3 operator()(float t) const;
    /** Offset the ray by epsilon in the direction of the ray */
    void offset(float const epsilon = 1e-3f);


private:
    /** Internal initial position of the ray */
    vec3 p0_data;
    /** Internal direction of the ray */
    vec3 u_data;
};

/** Return the reflected ray with respect to the normal n
    \return ray(r.p0() , r.u()- 2<r.u(),n> n)
    \note n should have a unit norm
*/
ray reflected(ray const& r,vec3 const& n);

/** Exporting ray data */
std::ostream& operator<<(std::ostream& stream,ray const& r);



}



#endif
