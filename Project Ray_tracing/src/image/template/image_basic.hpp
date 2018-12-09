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
#ifndef IMAGE_BASIC_HPP
#define IMAGE_BASIC_HPP

#include <iostream>
#include <vector>




namespace cpe
{

class ivec2;

/** Container for picture data storing generic values (color, depth, ...) . */
template <typename VALUE>
class image_basic
{
    public:

        // ********************************************* //
        // ********************************************* //
        //  CONSTRUCTORS
        // ********************************************* //
        // ********************************************* //

        /** Empty constructor */
        image_basic();
        /** \brief create square picture of size NxN
            \param unsigned int N: size (width & height) of the picture
        */
        image_basic(int N);
        /** \brief create picture of size Nx x Ny
            \param unsigned int Nx: picture width
            \param unsigned int Ny: picture height
        */
        image_basic(int Nx,int Ny);



        // ********************************************* //
        // ********************************************* //
        //  Picture Size
        // ********************************************* //
        // ********************************************* //

        /** Get the picture width in pixel */
        int Nx() const;
        /** Get the picture height in pixel */
        int Ny() const;
        /** Get the full size Nx x Ny of the image */
        int size() const;


        // ********************************************* //
        // ********************************************* //
        //  Pixel access
        // ********************************************* //
        // ********************************************* //

        /** Access the value at a given offset */
        VALUE const& operator[](int offset) const;
        /** Access/set the value at a given offset */
        VALUE& operator[](int offset);

        /** Accessor on a pixel at a given (kx,ky) */
        VALUE const& operator()(int x,int y) const;
        /** Accessor on a pixel at a given (kx,ky) */
        VALUE const& operator()(ivec2 const& index) const;

        /** Accessor on a pixel at a given (kx,ky) */
        VALUE& operator()(int x,int y);
        /** Accessor on a pixel at a given (kx,ky) */
        VALUE& operator()(ivec2 const& index);


        // ********************************************* //
        // ********************************************* //
        //  Special pixel filling
        // ********************************************* //
        // ********************************************* //

        /** Fill the entire picture with a given value */
        void fill(VALUE const& value);



        /** STL compatible ranged-for loop */
        typename std::vector<VALUE>::iterator begin();
        /** STL compatible ranged-for loop */
        typename std::vector<VALUE>::iterator end();
        /** STL compatible ranged-for loop */
        typename std::vector<VALUE>::const_iterator begin() const;
        /** STL compatible ranged-for loop */
        typename std::vector<VALUE>::const_iterator bend() const;
        /** STL compatible ranged-for loop */
        typename std::vector<VALUE>::const_iterator cbegin() const;
        /** STL compatible ranged-for loop */
        typename std::vector<VALUE>::const_iterator cbend() const;


    private:


        // ********************************************* //
        // ********************************************* //
        //  Internal data
        // ********************************************* //
        // ********************************************* //

        /** Internal data storage as std::vector of unsigned char

            Picture is stored as (color_0 , color_1 , ...) vector size is therefore 3 Nx Ny
        */
        std::vector<VALUE> data;

        /** Storage of picture width */
        int Nx_data;
        /** Storage of picture height */
        int Ny_data;


};



}

#include "image_basic.tpp"

#endif
