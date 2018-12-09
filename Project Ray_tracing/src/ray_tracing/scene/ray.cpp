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

#include "ray.hpp"
#include "lib/common/error_handling.hpp"
#include <cmath>

namespace cpe
{

ray::ray()
    :p0_data(vec3()),u_data(vec3(1.0f,0.0f,0.0f))
{}

ray::ray(vec3 const& p0_param,vec3 const& u_param)
    :p0_data(p0_param),u_data(normalized(u_param))
{}

vec3 const& ray::p0() const {return p0_data;}
vec3 const& ray::u() const {return u_data;}


vec3 ray::operator()(float const t) const
{
    return p0_data + t*u_data;
}

void ray::offset(float const epsilon)
{
    p0_data += epsilon*u_data;
}

ray reflected(ray const& r,vec3 const& n)
{
    ASSERT_CPE(std::abs(norm(n)-1.0f) < 1e-3f , "Normal should be of unit norm");

    return ray( r.p0() , r.u()-2*dot(r.u(),n)*n );
}

std::ostream& operator<<(std::ostream& stream,ray const& r)
{
    stream<<r.p0()<<" , "<<r.u();
    return stream;
}


}
