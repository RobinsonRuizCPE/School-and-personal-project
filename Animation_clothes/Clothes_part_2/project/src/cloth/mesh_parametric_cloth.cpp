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

#include "mesh_parametric_cloth.hpp"

#include "../lib/common/error_handling.hpp"
#include <cmath>

namespace cpe
{

vec3 mesh_parametric_cloth::unitary_spring_force(vec3 const p0, vec3 const p1, float const K, float const L)
{
    vec3 u = p0-p1;
    float L10 = norm(u);
    vec3 f = K * (L10-L) * u/L10;
    return f;
}




void mesh_parametric_cloth::update_force()
{

    int const Nu = size_u();
    int const Nv = size_v();
    int const N_total = Nu*Nv;
    ASSERT_CPE(static_cast<int>(force_data.size()) == Nu*Nv , "Error of size");


    //Gravity
    static vec3 const g (0.0f,0.0f,-9.81f);
    vec3 const g_normalized = g/N_total;
    /*
    for(int ku=0 ; ku<Nu ; ++ku)
    {
        for(int kv=0 ; kv<Nv ; ++kv)
        {
            force(ku,kv) = g_normalized;
        }
    }
    */

    //*************************************************************//
    // TO DO, Calculer les forces s'appliquant sur chaque sommet
    //*************************************************************//

    float const K = 20.0f;
    float const L_rest_struct = 1.0f/(float)(Nu-1);
    float const L_rest_shear = sqrt(2.f)*L_rest_struct;
    float const L_rest_bend = 2.f*L_rest_struct;

    std::vector<int> vois_struct = {-1,0, 1,0 ,0,1 ,0,-1};
    std::vector<int> vois_shear = {-1,-1, -1,1 ,1,-1 ,1,1};
    std::vector<int> vois_bend = {-2,0, 2,0 ,0,2 ,0,-2};

    float K_vent = 0.01f;
    vec3 u_vent = {0.9f,0.0f,0.2f};

    for(int ku=0 ; ku<Nu ; ++ku)
    {
        for(int kv=0 ; kv<Nv ; ++kv)
        {

            force(ku,kv) = g_normalized;

            //Structure
            for(int i=0; i<vois_struct.size(); i+=2)
            {
                int ku2 = ku + vois_struct[i];
                int kv2 = kv + vois_struct[i+1];
                if(ku2 >= 0 && ku2 < Nu && kv2 >= 0 && kv2 < Nv )
                {
                    force(ku,kv) += unitary_spring_force(vertex(ku2,kv2),vertex(ku,kv),K,L_rest_struct);
                }

            }

            //Shearing
            for(int i=0; i<vois_struct.size(); i+=2)
            {
                int ku2 = ku + vois_shear[i];
                int kv2 = kv + vois_shear[i+1];
                if(ku2 >= 0 && ku2 < Nu && kv2 >= 0 && kv2 < Nv )
                {
                    force(ku,kv) += unitary_spring_force(vertex(ku2,kv2),vertex(ku,kv),1.4*K,L_rest_shear);
                }

            }

            //Bend
            for(int i=0; i<vois_struct.size(); i+=2)
            {
                int ku2 = ku + vois_bend[i];
                int kv2 = kv + vois_bend[i+1];
                if(ku2 >= 0 && ku2 < Nu && kv2 >= 0 && kv2 < Nv )
                {
                    force(ku,kv) += unitary_spring_force(vertex(ku2,kv2),vertex(ku,kv),2*K,L_rest_bend);
                }

            }

            //VENT
            force(ku,kv) += K_vent*dot(normal(ku,kv),u_vent)*normal(ku,kv);


        }
    }



 /*
    for(int ku=0 ; ku<Nu ; ++ku)
    {
        for(int kv=0 ; kv<Nv ; ++kv)
        {


            if(ku > 0)
            {
                f += unitary_spring_force(vertex(ku,kv),vertex(ku-1,kv),K,L_rest_struct);
            }
            else
            {
                fT = vec3();
            }
            if(ku < Nu-1)
            {
                f += unitary_spring_force(vertex(ku,kv),vertex(ku+1,kv),K,L_rest_struct);
            }
            else
            {
                fB = vec3();
            }
            if(kv > 0)
            {
                f += unitary_spring_force(vertex(ku,kv),vertex(ku,kv-1),K,L_rest_struct);
            }
            else
            {
               f = vec3();
            }
            if(kv < Nv-1)
            {
                f += unitary_spring_force(vertex(ku,kv),vertex(ku,kv+1),K,L_rest_struct);
            }
            else
            {
                fR = vec3();
            }


            force(ku,kv) = f + g_normalized;
        }
    }
 */
    force(0,0) = vec3(0.0f,0.0f,0.0f);
    force(0,Nv-1) = vec3(0.0f,0.0f,0.0f);




    //*************************************************************//


}

void mesh_parametric_cloth::integration_step(float const dt)
{
    ASSERT_CPE(speed_data.size() == force_data.size(),"Incorrect size");
    ASSERT_CPE(static_cast<int>(speed_data.size()) == size_vertex(),"Incorrect size");


    int const Nu = size_u();
    int const Nv = size_v();
    //*************************************************************//
    // TO DO: Calculer l'integration numerique des positions au cours de l'intervalle de temps dt.
    //*************************************************************//

    static float const mu = 0.2f;
    vec3 sphere_center = {0.5f,0.05f,-1.0f};
    float sphere_radius = 0.2f;


    for(int ku=0; ku<Nu ; ++ku)
    {
        for(int kv=0 ; kv<Nv ; ++kv)
        {
            speed(ku,kv) = (1-mu*dt)*speed(ku,kv) + dt*force(ku,kv);
            vertex(ku,kv) = vertex(ku,kv) + dt*speed(ku,kv);

            //Collision Sphere
            vec3 inv_dir_norm_colli = normalized(vertex(ku,kv)-sphere_center);
            vec3 in_sphere = vertex(ku,kv)-sphere_center;

            if(norm(vertex(ku,kv)-sphere_center) <= sphere_radius )
            {
                   //float offset = sphere_radius - norm(vertex(ku,kv)-sphere_center);
                   vertex(ku,kv) =  sphere_center + sphere_radius*inv_dir_norm_colli ;
                   force(ku,kv) = force(ku,kv) - dot(force(ku,kv),inv_dir_norm_colli)*inv_dir_norm_colli;
                   speed(ku,kv) = speed(ku,kv) - dot(speed(ku,kv),inv_dir_norm_colli)*inv_dir_norm_colli;
            }


            //Collision Plan
            if(vertex(ku,kv).z() < -1.0f)
            {
                vertex(ku,kv).z() = -1.0f;
                force(ku,kv).z() = 0.0f;
                speed(ku,kv).z() = 0.0f;
            }
        }
    }

    //*************************************************************//







    //security check (throw exception if divergence is detected)
    static float const LIMIT=30.0f;
    for(int ku=0 ; ku<Nu ; ++ku)
    {
        for(int kv=0 ; kv<Nv ; ++kv)
        {
            vec3 const& p = vertex(ku,kv);

            if( norm(p) > LIMIT )
            {
                throw exception_divergence("Divergence of the system",EXCEPTION_PARAMETERS_CPE);
            }
        }
    }

}

void mesh_parametric_cloth::set_plane_xy_unit(int const size_u_param,int const size_v_param)
{
    mesh_parametric::set_plane_xy_unit(size_u_param,size_v_param);

    int const N = size_u()*size_v();
    speed_data.resize(N);
    force_data.resize(N);
}

vec3 const& mesh_parametric_cloth::speed(int const ku,int const kv) const
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(speed_data.size()),"Internal error");

    return speed_data[offset];
}

vec3& mesh_parametric_cloth::speed(int const ku,int const kv)
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(speed_data.size()),"Internal error");

    return speed_data[offset];
}

vec3 const& mesh_parametric_cloth::force(int const ku,int const kv) const
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(force_data.size()),"Internal error");

    return force_data[offset];
}

vec3& mesh_parametric_cloth::force(int const ku,int const kv)
{
    ASSERT_CPE(ku >= 0 , "Value ku ("+std::to_string(ku)+") should be >=0 ");
    ASSERT_CPE(ku < size_u() , "Value ku ("+std::to_string(ku)+") should be < size_u ("+std::to_string(size_u())+")");
    ASSERT_CPE(kv >= 0 , "Value kv ("+std::to_string(kv)+") should be >=0 ");
    ASSERT_CPE(kv < size_v() , "Value kv ("+std::to_string(kv)+") should be < size_v ("+std::to_string(size_v())+")");

    int const offset = ku + size_u()*kv;

    ASSERT_CPE(offset < static_cast<int>(force_data.size()),"Internal error");

    return force_data[offset];
}


}
