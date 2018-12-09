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

#ifndef ANTI_ALIASING_TABLE_HPP
#define ANTI_ALIASING_TABLE_HPP

#include <vector>

namespace cpe
{

/** Precompute a table of muli-sampling per pixel associated with a Gaussian kernel
    Store a table displacement and weight such that

    weight(kx,ky) = K exp( - (dx^2+dy^2) / sigma^2 )
    with  dx = displacement(kx) = kx/(N_sample-1)
          dy = displacement(ky) = ky/(N_sample-1)
    and    K = 1 / (sum_{dx,dy} exp( - (dx^2+dy^2) / sigma^2 ) )


    Usage example:
    -----------------------------

    ** 1. Example case without anti-aliasing **
    //loop over (u,v) in [0,1]
    {
        value = F(u,v); //F is the function to evaluate
        ...
    }

    ** 2. Example case with anti-aliasing **
    int const N_sample = 5;
    anti_aliasing_table aa(N_sample);

    //loop over (u,v) in [0,1]
    {
        //init value = 0
        for(int dx=0 ; dx<N_sample ; ++dx)
        {
            for(int dy=0 ; dy<N_sample ; ++dy)
            {
                float const du = aa.displacement(dx)/(Nx-1); //Nx is the size in pixel in x direction
                float const dv = aa.displacement(dy)/(Ny-1); //Ny is the size in pixel in y direction

                float const w = aa.weight(dx,dy);

                value += w*F(u+du,v+dv); //F is a function of (u,v)
            }
        }

        //value contains the gaussian weighted average around (u,v) position
        // ...
    }



*/
class anti_aliasing_table
{
public:

    /** Precompute displacement and weights for anti aliasing filter
        \param N_sample: Number of sampling per pixel (should be an odd number to sample the middle position)
        \param distance: The distance (in pixel number) from which the sampling is performed
        \param sigma: The weighting factor of the gaussian kernel
    */
    anti_aliasing_table(int N_sample=5,float distance=1.0f,float sigma=0.8f);

    /** The displacement to applied in pixel unit */
    float displacement(int k) const;
    /** The weighting factor */
    float weight(int kx,int ky) const;


private:

    /** Storage for the displacement (1D vector of N_sample values) */
    std::vector <float> displacement_data;
    /** Storage for the displacement (2D vector of N_sample^2 values) */
    std::vector <float> weight_data;
};

}

#endif
