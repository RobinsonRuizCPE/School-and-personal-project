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

#include "intersection_data.hpp"

namespace cpe
{

intersection_data::intersection_data()
    :position(vec3()),normal(vec3(0,0,1)),relative(-1.0f)
{}

intersection_data::intersection_data(vec3 const& position_param,
                      vec3 const& normal_param,
                      float relative_param)
    :position(position_param),normal(normal_param),relative(relative_param)
{}

void intersection_data::set(vec3 const& position_param,
         vec3 const& normal_param,
         float relative_param)
{
    position = position_param;
    normal   = normal_param;
    relative = relative_param;
}


std::ostream& operator<<(std::ostream& stream,intersection_data const& intersection)
{
    stream<<intersection.position<<" "<<intersection.normal<<" "<<intersection.relative;
    return stream;
}

}
