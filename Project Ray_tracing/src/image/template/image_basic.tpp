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


#include "image_basic.hpp"
#include "../ivec2.hpp"

#include "lib/common/error_handling.hpp"

namespace cpe
{

template <typename VALUE>
image_basic<VALUE>::image_basic()
    :data(),Nx_data(0),Ny_data(0)
{}

template <typename VALUE>
image_basic<VALUE>::image_basic(int const N)
    :data(N*N),Nx_data(N),Ny_data(N)
{}

template <typename VALUE>
image_basic<VALUE>::image_basic(int const Nx,int const Ny)
    :data(Nx*Ny),Nx_data(Nx),Ny_data(Ny)
{}



template <typename VALUE>
int image_basic<VALUE>::Nx() const
{
    return Nx_data;
}

template <typename VALUE>
int image_basic<VALUE>::Ny() const
{
    return Ny_data;
}

template <typename VALUE>
int image_basic<VALUE>::size() const
{
    return Nx_data*Ny_data;
}

template <typename VALUE>
VALUE const& image_basic<VALUE>::operator[](int const offset) const
{
    ASSERT_CPE(offset>=0 && offset<static_cast<int>(data.size()),"Offset="+std::to_string(offset)+" out of limit [0,"+std::to_string(data.size())+"]");
    return data[offset];
}

template <typename VALUE>
VALUE& image_basic<VALUE>::operator[](int const offset)
{
    ASSERT_CPE(offset>=0 && offset<static_cast<int>(data.size()),"Offset="+std::to_string(offset)+" out of limit [0,"+std::to_string(data.size())+"]");
    return data[offset];
}

template <typename VALUE>
VALUE const& image_basic<VALUE>::operator()(int const x,int const y) const
{
    ASSERT_CPE(x>=0 && x<Nx() && y>=0 && y<Ny(),"(x,y)=("+std::to_string(x)+","+std::to_string(y)+") out of limit [Nx="+std::to_string(Nx())+" , Ny="+std::to_string(Ny())+"]");

    int const offset = x+Nx()*y;
    return (*this)[offset];
}

template <typename VALUE>
VALUE const& image_basic<VALUE>::operator()(ivec2 const& index) const
{
    return (*this)(index.x(),index.y());
}

template <typename VALUE>
VALUE& image_basic<VALUE>::operator()(int const x,int const y)
{
    ASSERT_CPE(x>=0 && x<Nx() && y>=0 && y<Ny(),"(x,y)=("+std::to_string(x)+","+std::to_string(y)+") out of limit [Nx="+std::to_string(Nx())+" , Ny="+std::to_string(Ny())+"]");

    int const offset = x+Nx()*y;
    return (*this)[offset];
}

template <typename VALUE>
VALUE& image_basic<VALUE>::operator()(ivec2 const& index)
{
    return (*this)(index.x(),index.y());
}

template <typename VALUE>
void image_basic<VALUE>::fill(VALUE const& value)
{
    for(VALUE& current : data)
        current = value;
}

template <typename VALUE>
typename std::vector<VALUE>::iterator image_basic<VALUE>::begin() { return data.begin(); }

template <typename VALUE>
typename std::vector<VALUE>::iterator image_basic<VALUE>::end()   { return data.end(); }

template <typename VALUE>
typename std::vector<VALUE>::const_iterator image_basic<VALUE>::begin() const  { return data.cbegin(); }

template <typename VALUE>
typename std::vector<VALUE>::const_iterator image_basic<VALUE>::bend() const   { return data.cend(); }

template <typename VALUE>
typename std::vector<VALUE>::const_iterator image_basic<VALUE>::cbegin() const { return data.cbegin(); }

template <typename VALUE>
typename std::vector<VALUE>::const_iterator image_basic<VALUE>::cbend() const  { return data.cend(); }

}


