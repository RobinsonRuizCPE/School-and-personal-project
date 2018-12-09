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

#include "ivec2.hpp"

#include "lib/common/error_handling.hpp"

namespace cpe
{

ivec2::ivec2()
    :x_data(0),y_data(0)
{}

ivec2::ivec2(int const x_param,int const y_param)
    :x_data(x_param),y_data(y_param)
{}

ivec2::ivec2(std::initializer_list<int> const& arg)
    :x_data(0),y_data(0)
{
    ASSERT_CPE(arg.size()>=2,"initializer list should have at least 2 arguments");

    int counter=0;
    for(int v : arg)
    {
        if(counter==0)
            x_data=v;
        else if(counter==1)
            y_data=v;
        counter++;
    }
}



int ivec2::x() const  {return x_data;}
int& ivec2::x()       {return x_data;}
int ivec2::y() const  {return y_data;}
int& ivec2::y()       {return y_data;}


std::ostream& operator<<(std::ostream& stream,ivec2 const& u)
{
    stream<<"("<<u.x()<<" ; "<<u.y()<<")";
    return stream;
}

std::string to_string(ivec2 const& u)
{
    return std::to_string(u.x())+","+std::to_string(u.y());
}

ivec2& operator+=(ivec2& u1,ivec2 const& u2)
{
    u1.x()+=u2.x();
    u1.y()+=u2.y();
    return u1;
}
ivec2& operator-=(ivec2& u1,ivec2 const& u2)
{
    u1.x()-=u2.x();
    u1.y()-=u2.y();
    return u1;
}

ivec2 operator+(ivec2 const& u1,ivec2 const& u2)
{
    ivec2 u=u1;
    u+=u2;
    return u;
}
ivec2 operator-(ivec2 const& u1,ivec2 const& u2)
{
    ivec2 u=u1;
    u-=u2;
    return u;
}

bool operator==(ivec2 const& u1, ivec2 const& u2)
{
    return u1.x()==u2.x() && u1.y()==u2.y();
}

bool operator!=(ivec2 const& u1, ivec2 const& u2)
{
    return !(u1==u2);
}


}
