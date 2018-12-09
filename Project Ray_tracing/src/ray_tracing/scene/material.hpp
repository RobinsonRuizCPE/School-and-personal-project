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

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "shading_parameter.hpp"
#include "image/color.hpp"

namespace cpe
{


/** \brief material associated to a given object (color+shading data) */
struct material
{
    material();
    material(color const& color_param,
             shading_parameter const& shading_param,
             float reflection_param);

    /** Basic color of the object */
    color color_object() const;

    /** Shading parameters of the object */
    shading_parameter shading() const;

    /** Reflection power of the object */
    float reflection() const;

private:

    /** Original color of the object */
    color color_data;
    /** Shading parameter to compute illumination */
    shading_parameter shading_data;
    /** Amount of reflection (between 0 to 1),
     *  0: no reflection
     *  1: perfect mirror  */
    float reflection_data;
};
}

#endif
