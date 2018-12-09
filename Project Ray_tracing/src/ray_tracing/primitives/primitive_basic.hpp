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

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP


#include <vector>


namespace cpe
{
class ray;
struct intersection_data;


/** Generic container of a shape with intersect method */
class primitive_basic
{
public:


    primitive_basic();
    virtual ~primitive_basic();

    /** Method to call on a generic object to get intersection with a ray */
    virtual bool intersect(ray const& ray_param,intersection_data& intersection) const = 0;

};




}

#endif
