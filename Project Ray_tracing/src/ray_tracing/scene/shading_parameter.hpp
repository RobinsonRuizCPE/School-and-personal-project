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

#ifndef SHADING_PARAMETER
#define SHADING_PARAMETER

namespace cpe
{
struct color;
class vec3;


struct shading_parameter
{
    /** \brief direct constructor for the shading parameters */
    shading_parameter(float ambiant_param=0.1f,
                      float diffuse_param=0.6f,
                      float specular_param=0.4f,
                      float specular_exponent=2.0f);


    /** ambiant component */
    float ambiant;
    /** diffuse component */
    float diffuse;
    /** specular component */
    float specular;
    /** specular exponent */
    float specular_exponent;
};

/** Compute the shading associate to a given position on a surface
    \param shading: the shading parameter of the object
    \param p: the current 3D position
    \param p_light: the position of the light source
    \param normal: the normal of the surface
    \param p_camera: the center of the camera/eye
*/
color compute_shading(shading_parameter const& shading,
                      color const& input,
                      vec3 const& p,
                      vec3 const& p_light,
                      vec3 const& normal,
                      vec3 const& p_camera);
}

#endif
