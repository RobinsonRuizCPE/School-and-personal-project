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
#ifndef IMAGE_PPM_HPP
#define IMAGE_PPM_HPP

#include <string>

namespace cpe
{
struct image;
struct image_zbuffer;



/** \brief Export the picture into ppm format (ascii)
    \param std::string filename: file to store the picture
    \param image pic: image to store */
void export_ppm(std::string const& filename,image const& pic);

/** \brief Export the z-buffer as a gray picture into ppm format (ascii)
    \param std::string filename: file to store the picture
    \param z_buffer buffer: depth-map to store */
void export_ppm(std::string const& filename,image_zbuffer const& zbuffer);

/** \brief read a ppm picture format
    \param std::string filename: path of the file to read */
image read_ppm(const std::string& filename);

}
#endif
