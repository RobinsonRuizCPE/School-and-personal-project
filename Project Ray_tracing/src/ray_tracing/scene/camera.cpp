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


#include "camera.hpp"


namespace cpe
{

camera::camera()
    :center_data(vec3(0.0f,0.0f,-2.0f)),
      direction_data(vec3(0.0f,0.0f,1.0f)),
      up_data(vec3(0.0f,1.0f,0.0f)),
      distance_screen_data(2.0f),
      length_screen_data(4.0f)
{}

camera::camera(vec3 const& center_param,
       vec3 const& direction_param,
       vec3 const& up_param,
       float const distance_screen_param,
       float const length_screen_param)
    :center_data(center_param),
      direction_data(direction_param),
      up_data(up_param),
      distance_screen_data(distance_screen_param),
      length_screen_data(length_screen_param)
{}


vec3 const& camera::center() const {return center_data;}
vec3 const& camera::direction() const {return direction_data;}
vec3 const& camera::up() const {return up_data;}
float camera::distance_screen() const {return distance_screen_data;}
float camera::length_screen() const {return length_screen_data;}


vec3 screen_position(camera const& cam,float u,float v)
{

    vec3 const right = normalized(cross(cam.direction(),cam.up()));
    vec3 const p0    = cam.center() + cam.direction() * cam.distance_screen();

    vec3 const translate_right = right * (u-0.5f)*2.0f*cam.length_screen();
    vec3 const translate_up    = cam.up() * (v-0.5f)*2.0f*cam.length_screen();

    vec3 const current = p0+translate_right+translate_up;

    return current;
}

}
