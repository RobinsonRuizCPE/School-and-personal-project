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
#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

namespace cpe
{


/** \brief Container class for (r,g,b) color. Each component is between [0,1] */
struct color
{


    // ********************************************* //
    // ********************************************* //
    //  CONSTRUCTORS
    // ********************************************* //
    // ********************************************* //

    color();
    /** Direct constructor with gray value (r,b,g)=(gray,gray,gray) */
    color(float gray);
    /** Direct constructor with (r,b,g) value */
    color(float r,float g,float b);
    /** Direct constructor from initializer list */
    color(std::initializer_list<float> const& param);


    // ********************************************* //
    // ********************************************* //
    //  ACCESS COLOR VALUE
    // ********************************************* //
    // ********************************************* //

    /** Get red value */
    float r() const;
    /** Get/set red value */
    float& r();
    /** Get green value */
    float g() const;
    /** Get/set green value */
    float& g();
    /** Get blue value */
    float b() const;
    /** Get/set blue value */
    float& b();


private:

    // ********************************************* //
    // ********************************************* //
    //  INTERNAL STORAGE
    // ********************************************* //
    // ********************************************* //

    /** Internal red component */
    float r_data;
    /** Internal green component */
    float g_data;
    /** Internal blue component */
    float b_data;

};

/** Add two colors */
color& operator+=(color& c0,color const& c1);
/** Substract two colors */
color& operator-=(color& c0,color const& c1);
/** Add two colors */
color operator+(color const& c0,color const& c1);
/** Substract two colors */
color operator-(color const& c0,color const& c1);
/** Multiply color by scalar */
color& operator*=(color& c0,float alpha);
/** Divide color by scalar */
color& operator/=(color& c0,float alpha);
/** Multiply color by scalar */
color operator*(color const& c0,float alpha);
/** Multiply color by scalar */
color operator*(float alpha,color const& c0);
/** Divide color by scalar */
color operator/(color const& c0,float alpha);

/** Mulitply two color : Componentwise product */
color& operator*=(color& c0,color const& c1);
/** Mulitply two color : Componentwise product */
color operator*(color const& c0,color const& c1);


// ********************************************* //
// ********************************************* //
//  I/O
// ********************************************* //
// ********************************************* //

/** \brief standard output */
std::ostream& operator<<(std::ostream& stream,color const& c);

/** clamp the color between 0 and 1 */
color clamp(color const& c);

}

#endif
