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

#include "image_ppm.hpp"

#include "lib/common/exception_cpe.hpp"
#include "../image.hpp"
#include "../image_zbuffer.hpp"

#include <fstream>
#include <sstream>

namespace cpe
{

void export_ppm(std::string const& filename,image_zbuffer const& pic)
{
    std::ofstream stream(filename.c_str(),std::ofstream::out);
    if(!stream.good())
        throw cpe::exception_cpe("Error in opening file "+filename+"... : cannot open file ",EXCEPTION_PARAMETERS_CPE);

    int const Nx = pic.Nx();
    int const Ny = pic.Ny();

    //magic number
    stream<<"P3 \n";
    //size
    stream<<Nx<<" "<<Ny<<" \n";
    //color number
    stream<<"255"<<std::endl;

    std::stringstream str;
    for(int k2=0 ; k2<Ny ; ++k2)
    {
        for(int k1=0 ; k1<Nx ; ++k1)
        {
            //get the color and clamp it just in case
            float const z = 0.5*(pic(k1,k2)+1.0f);

            //write it in [0,255]
            int const r = static_cast<int>(255*z);
            int const g = static_cast<int>(255*z);
            int const b = static_cast<int>(255*z);
            str<<r<<" "<<g<<" "<<b<<std::endl;
        }
    }
    stream<<str.str();

    stream.close();
}

void export_ppm(std::string const& filename,image const& pic)
{
    std::ofstream stream(filename.c_str(),std::ofstream::out);
    if(!stream.good())
        throw cpe::exception_cpe("Error in opening file "+filename+"... : cannot open file ",EXCEPTION_PARAMETERS_CPE);

    int const Nx = pic.Nx();
    int const Ny = pic.Ny();

    //magic number
    stream<<"P3 \n";
    //size
    stream<<Nx<<" "<<Ny<<" \n";
    //color number
    stream<<"255"<<std::endl;

    std::stringstream str;
    for(int k2=0 ; k2<Ny ; ++k2)
    {
        for(int k1=0 ; k1<Nx ; ++k1)
        {
            //get the color and clamp it just in case
            color c = clamp(pic(k1,k2));

            //write it in [0,255]
            int const r = static_cast<int>(255*c.r());
            int const g = static_cast<int>(255*c.g());
            int const b = static_cast<int>(255*c.b());
            str<<r<<" "<<g<<" "<<b<<std::endl;
        }
    }
    stream<<str.str();

    stream.close();
}

image read_ppm(const std::string& filename)
{
    std::ifstream stream(filename.c_str());
    if(!stream.good())
        throw exception_cpe("Error in opening file ["+filename+"] ... : cannot open file ",EXCEPTION_PARAMETERS_CPE);

    //go to magic number
    std::string buffer;
    buffer="#";
    while(buffer[0]=='#')
        std::getline(stream,buffer);
    if(buffer.find("P3")==std::string::npos)
        throw exception_cpe("Cannot find P3 magic number in file "+filename,EXCEPTION_PARAMETERS_CPE);

    //(Nx,Ny)
    buffer="#";
    while(buffer[0]=='#')
        std::getline(stream,buffer);
    std::istringstream tokens(buffer);

    int temp_Nx=0,temp_Ny=0;
    tokens>>temp_Nx;tokens>>temp_Ny;

    //color number
    stream>>buffer;

    image new_pic(temp_Nx,temp_Ny);

    for(int ky=0 ; ky<temp_Ny ; ky++)
    {
        for(int kx=0 ; kx<temp_Nx ; kx++)
        {
            //read 3 values from file
            int val[3];
            for(int k_dim=0;k_dim<3;++k_dim)
                stream >> val[k_dim];

            //convert the values to (r,g,b) component between [0,1]
            float const r = val[0]/255.0f;
            float const g = val[1]/255.0f;
            float const b = val[2]/255.0f;

            new_pic(kx,ky) = {r,g,b};

        }
    }


    stream.close();

    return new_pic;

}


}
