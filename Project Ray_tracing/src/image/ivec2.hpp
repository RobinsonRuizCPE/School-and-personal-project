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



#ifndef IVEC2_HPP
#define IVEC2_HPP

#include <iostream>


namespace cpe
{
/** \brief Container for 2D index positions (pixel position) */
class ivec2
{
public:

    ivec2();
    ivec2(int x_param,int y_param);
    ivec2(std::initializer_list<int> const& param);

    // ********************************************* //
    // ********************************************* //
    //  Coordinate access
    // ********************************************* //
    // ********************************************* //

    /** get x-coordinate */
    int x() const;
    /** get x-coordinate */
    int& x();
    /** get y-coordinate */
    int y() const;
    /** get y-coordinate */
    int& y();


private:

    // ********************************************* //
    // ********************************************* //
    //  Internal storage
    // ********************************************* //
    // ********************************************* //

    /** Internal x-coordinate */
    int x_data;
    /** Internal y-coordinate */
    int y_data;

};


// ********************************************* //
// ********************************************* //
//  Operators
// ********************************************* //
// ********************************************* //

/** Sum between two positions */
ivec2 operator+(ivec2 const& u1,ivec2 const& u2);
/** Difference between two positions */
ivec2 operator-(ivec2 const& u1,ivec2 const& u2);
/** Internal sum between two positions */
ivec2& operator+=(ivec2& u1,ivec2 const& u2);
/** Internal diff between two positions */
ivec2& operator-=(ivec2& u1,ivec2 const& u2);
/** Equality operator */
bool operator==(ivec2 const& u1, ivec2 const& u2);
/** Nequality operator */
bool operator!=(ivec2 const& u1, ivec2 const& u2);

// ********************************************* //
// ********************************************* //
//  I/O
// ********************************************* //
// ********************************************* //

/** Standard output */
std::ostream& operator<<(std::ostream& stream,ivec2 const& u);

std::string to_string(ivec2 const& u);


}


#endif
