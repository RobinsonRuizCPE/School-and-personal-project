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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "lib/3d/vec3.hpp"

namespace cpe
{

/** Storage of a camera and screen parameters */
class camera
{
    public:

        // ********************************************* //
        //  Constructor
        // ********************************************* //

        camera();

        /** Camera parameterized by its center, its main direction, and its up direction,
            the distance between the screen and the center, and the length of the screen */
        camera(vec3 const& center_param,
               vec3 const& direction_param,
               vec3 const& up_param,
               float distance_screen_param,
               float length_screen_param);

        // ********************************************* //
        //  Access parameters
        // ********************************************* //

        /** Get the center of the camera */
        vec3 const& center() const;
        /** Get the direction value */
        vec3 const& direction() const;
        /** Get the up direction value */
        vec3 const& up() const;
        /** Get the dist_screen value */
        float distance_screen() const;
        /** Get the length_of the screen value */
        float length_screen() const;



    private:

        /** Internal camera center */
        vec3 center_data;
        /** Direction of pointing camera */
        vec3 direction_data;
        /** Up direction of the camera */
        vec3 up_data;
        /** Internal screen distance to the center */
        float distance_screen_data;
        /** Length of the screen */
        float length_screen_data;
};

/** Compute the 3D position of the pixel parameterized by (u,v), where (u,v) is in [0,1] on the screen of the camera */
vec3 screen_position(camera const& cam,float u,float v);


}

#endif
