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

#include "material.hpp"



namespace cpe
{
material::material()
    :color_data(color(1.0f,1.0f,1.0f)),
      shading_data(shading_parameter()),
      reflection_data(0.0f)
{}

material::material(color const& color_param,
                   shading_parameter const& shading_param,
                   float const reflection_param)
    :color_data(color_param),shading_data(shading_param),reflection_data(reflection_param)
{

}

color material::color_object() const
{
    return color_data;
}
shading_parameter material::shading() const
{
    return shading_data;
}
float material::reflection() const
{
    return reflection_data;
}

}
