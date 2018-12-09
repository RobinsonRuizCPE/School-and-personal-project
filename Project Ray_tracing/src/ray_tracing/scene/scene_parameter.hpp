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

#ifndef SCENE_HPP
#define SCENE_HPP

#include "../primitives/primitive_basic.hpp"
#include "material.hpp"
#include "light.hpp"
#include "camera.hpp"

#include <vector>
#include <memory>



namespace cpe
{

/** Container of a 3D scene (primitives and their materials, lights, and a camera) */
class scene_parameter
{
public:

    scene_parameter();
    ~scene_parameter();

    /** Add a primitive and its material
     * \note the primitive must be a pointer dynamically allocated. It will be released in the scene destructor.
     */
    void push_back(primitive_basic* const primitive,
                   material const& mat);
    /** Add a light to the scene */
    void push_back(light const& l);

    /** Get a primitive by its index */
    primitive_basic const& get_primitive(int index) const;
    /** Get a material by its index */
    material const& get_material(int index) const;

    /** Get a light by its index */
    light const& get_light(int index) const;

    /** Number of primitives */
    int size_primitive() const;
    /** Number of light */
    int size_light() const;

    /** Get the camera of the scene */
    camera const& get_camera() const;
    /** Set the camera of the scene */
    void set_camera(camera const& camera_param);


private:

    /** Internal storage of the primitives */
    std::vector<primitive_basic*> primitive_data;
    /** Internal storage of the material */
    std::vector<material> material_data;
    /** Internal storage of the light */
    std::vector<light> light_data;
    /** Internal storage of the camera */
    camera camera_data;

};

}


#endif
