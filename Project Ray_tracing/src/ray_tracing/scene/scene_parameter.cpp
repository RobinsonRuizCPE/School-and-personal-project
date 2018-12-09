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

#include "scene_parameter.hpp"

#include "lib/common/error_handling.hpp"

namespace cpe
{

scene_parameter::scene_parameter()
    :primitive_data(),material_data(),light_data()
{}

scene_parameter::~scene_parameter()
{
    for(primitive_basic* p : primitive_data)
    {
        delete p;
        p=nullptr;
    }
}

void scene_parameter::push_back(primitive_basic* const primitive,
                      material const& mat)
{
    ASSERT_CPE(primitive!=nullptr,"Null pointer primitive");
    primitive_data.push_back(primitive);
    material_data.push_back(mat);
}

void scene_parameter::push_back(light const& l)
{
    light_data.push_back(l);
}

int scene_parameter::size_primitive() const
{
    return primitive_data.size();
}
int scene_parameter::size_light() const
{
    return light_data.size();
}

primitive_basic const& scene_parameter::get_primitive(int const index) const
{
    ASSERT_CPE(index>=0 && index<static_cast<int>(primitive_data.size()),"index "+std::to_string(index)+" is incorrect (should be < "+std::to_string(primitive_data.size()));
    return *( primitive_data[index] );
}
material const& scene_parameter::get_material(int const index) const
{
    ASSERT_CPE(index>=0 && index<static_cast<int>(material_data.size()),"index "+std::to_string(index)+" is incorrect (should be < "+std::to_string(material_data.size()));
    return material_data[index];
}
light const& scene_parameter::get_light(int const index) const
{
    ASSERT_CPE(index>=0 && index<static_cast<int>(light_data.size()),"index "+std::to_string(index)+" is incorrect (should be < "+std::to_string(light_data.size()));
    return light_data[index];
}

camera const& scene_parameter::get_camera() const
{
    return camera_data;
}
void scene_parameter::set_camera(camera const& camera_param)
{
    camera_data = camera_param;
}

}
