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

#include "color.hpp"

#include "lib/common/error_handling.hpp"
#include "lib/common/basic_functions.hpp"
#include <cmath>

namespace cpe
{
color::color()
    :r_data(0),g_data(0),b_data(0)
{}

color::color(float const gray)
    :r_data(gray),g_data(gray),b_data(gray)
{}

color::color(float const r,float const g,float const b)
    :r_data(r),g_data(g),b_data(b)
{}

color::color(std::initializer_list<float> const& param)
    :r_data(0),g_data(0),b_data(0)
{
    ASSERT_CPE(param.size()>=3,"Initializer list must have a size >= 3");

    int counter=0;
    for(float value : param)
    {
        switch(counter)
        {
        case 0: r_data=value; break;
        case 1: g_data=value; break;
        case 2: b_data=value; break;
        }

        counter++;
    }
}


float color::r() const  { return r_data; }
float& color::r()       { return r_data; }
float color::g() const  { return g_data; }
float& color::g()       { return g_data; }
float color::b() const  { return b_data; }
float& color::b()       { return b_data; }

color& operator+=(color& c0,color const& c1)
{
    c0.r()+=c1.r();
    c0.g()+=c1.g();
    c0.b()+=c1.b();
    return c0;
}

color& operator-=(color& c0,color const& c1)
{
    c0.r()-=c1.r();
    c0.g()-=c1.g();
    c0.b()-=c1.b();
    return c0;
}

color operator+(color const& c0,color const& c1)
{
    color temp=c0;
    temp+=c1;
    return temp;
}

color operator-(color const& c0,color const& c1)
{
    color temp=c0;
    temp-=c1;
    return temp;
}

color& operator*=(color& c0,float const alpha)
{
    c0.r()*=alpha;
    c0.g()*=alpha;
    c0.b()*=alpha;
    return c0;
}

color& operator/=(color& c0,float const alpha)
{
    ASSERT_CPE(std::abs(alpha)>1e-5,"Cannot divide by 0");

    c0.r()/=alpha;
    c0.g()/=alpha;
    c0.b()/=alpha;
    return c0;
}

color operator*(color const& c0,float const alpha)
{
    color temp=c0;
    temp*=alpha;
    return temp;
}
color operator*(float const alpha,color const& c0)
{
    return c0*alpha;
}

color operator/(color const& c0,float const alpha)
{
    ASSERT_CPE(std::abs(alpha)>1e-5,"Cannot divide by 0");

    color temp=c0;
    temp/=alpha;
    return temp;
}

std::ostream& operator<<(std::ostream& stream,color const& c)
{
    stream<<c.r()<<" "<<c.g()<<" "<<c.b();
    return stream;
}

color& operator*=(color& c0,color const& c1)
{
    c0.r() *= c1.r();
    c0.g() *= c1.g();
    c0.b() *= c1.b();

    return c0;
}

color operator*(color const& c0,color const& c1)
{
    color temp=c0;
    temp*=c1;
    return temp;
}

//truncate each color component between 0 and 1
color clamp(color const& c)
{
    return color( clamp(c.r()) , clamp(c.g()) , clamp(c.b()) );
}

}


