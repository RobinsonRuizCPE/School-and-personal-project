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

#include "anti_aliasing_table.hpp"

#include "lib/common/error_handling.hpp"

#include <cmath>

namespace cpe
{

anti_aliasing_table::anti_aliasing_table(int const N_sample,float const distance,float const sigma)
{
    ASSERT_CPE(N_sample>0,"N_supersample should be > 0");
    ASSERT_CPE(distance>0,"distance should be > 0");

    if(N_sample==1)
    {
        displacement_data.push_back(0);
        weight_data.push_back(1);
        return ;
    }

    for(int k=0 ; k<N_sample ; ++k)
    {
        float const u_local = static_cast<float>(k)/(N_sample-1);
        float const u = 2.0f * (u_local-0.5f);

        float const dL = distance*u;
        displacement_data.push_back(dL);
    }

    float const sigma2 = sigma*sigma;
    float const distance2 = distance*distance;
    float const normalization_factor = sigma2*distance2;

    float w_total = 0.0f;
    for(int kx=0 ; kx<N_sample ; ++kx)
    {
        float const dx = displacement_data[kx];
        for(int ky=0 ; ky<N_sample ; ++ky)
        {
            float const dy = displacement_data[ky];

            float const exponent = (dx*dx+dy*dy)/normalization_factor;
            float const w = std::exp(-exponent);

            w_total += w;
            weight_data.push_back(w);
        }
    }

    //normalization
    for(float& w : weight_data)
        w /= w_total;

}

float anti_aliasing_table::displacement(int k) const
{
    ASSERT_CPE(k>=0 && k<static_cast<int>(displacement_data.size()),"Incorrect offset for anti aliasing table");
    return displacement_data[k];
}
float anti_aliasing_table::weight(int kx,int ky) const
{
    int const N = displacement_data.size();

    ASSERT_CPE(kx>=0 && kx<N,"Incorrect offset for anti aliasing table");
    ASSERT_CPE(ky>=0 && ky<N,"Incorrect offset for anti aliasing table");
    ASSERT_CPE(ky+N*kx<static_cast<int>(weight_data.size()),"Incorrect offset for anti aliasing table");

    return weight_data[ky+N*kx];
}



}
