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

#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

namespace cpe
{
struct image;
class scene_parameter;
class ray;
struct intersection_data;
struct color;
struct material;
class camera;
class vec3;

/** Render the scence on the image */
void render(image& im,scene_parameter const& scene);

/** Compute the final color associated to given ray within the scene */
color ray_trace(ray const& r,scene_parameter const& scene);

/** Compute the first intersection encountered between a ray and a primitive of the scene */
bool compute_intersection(ray const& r,
                          scene_parameter const& scene,
                          intersection_data& intersection,
                          int& index_intersected_primitive);


/** Check if a point p is in the shadow of a given light source position for a given scene */
bool is_in_shadow(vec3 const& p,vec3 const& p_light, scene_parameter const& scene);

/** Compute the illumination at given intersection position */
color compute_illumination(material const& mat,
                           intersection_data const& intersection,
                           scene_parameter const& scene);


/** Generate a ray at the (u,v) point of the screen (u,v) \in [0,1] */
ray ray_generator(camera const& cam,float u,float v);

}
#endif
