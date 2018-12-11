
/** TP 5ETI - CPE Lyon - 2015/2016 */

#pragma once

#ifndef SCENE_HPP
#define SCENE_HPP

#include <GL/gl.h>
#include <GL/glew.h>

#include <QTime>

#include "../../lib/3d/mat3.hpp"
#include "../../lib/3d/vec3.hpp"
#include "../../lib/mesh/mesh.hpp"
#include "../../lib/opengl/mesh_opengl.hpp"
#include "../../lib/interface/camera_matrices.hpp"



#include <vector>


class myWidgetGL;

class scene
{
public:

    scene();



    /**  Method called only once at the beginning (load off files ...) */
    void load_scene();

    /**  Method called at every frame */
    void draw_scene();

    /** Set the pointer to the parent Widget */
    void set_widget(myWidgetGL* widget_param);


private:


    /** Load a texture from a given file and returns its id */
    GLuint load_texture_file(std::string const& filename);

    /** Access to the parent object */
    myWidgetGL* pwidget;

    /** Default id for the texture (white texture) */
    GLuint texture_default;


    /** Ground mesh */
    cpe::mesh mesh_ground;
    /** Ground mesh for OpenGL drawing */
    cpe::mesh_opengl mesh_ground_opengl;


    /** Mesh of a sphere */
    cpe::mesh mesh_sphere;
    /** OpenGL VBO for the sphere */
    cpe::mesh_opengl mesh_sphere_opengl;

    /** OpenGL ID for shader drawing meshes */
    GLuint shader_mesh;
    /** OpenGL ID for shader drawing meshes */
    GLuint shader_sphere;

    /** OpenGL ID for the texture of the ground */
    GLuint texture_ground;


    /** Time counter */
    QTime time_integration;

    /** Setup the shader for the mesh */
    void setup_shader_mesh(GLuint shader_id);

    /** The time interval for the numerical integration */
    float delta_t;

    /** Position of point 0 */
    cpe::vec3 p0;
    /** Position of point 1 */
    cpe::vec3 p1;
    /** Position of point 2 */
    cpe::vec3 p2;

    /** Rest length of spring 0-1 */
    float L10_rest;

    /** Speed of spring 1 */
    cpe::vec3 v1;
    /** Speed of spring 2*/
    cpe::vec3 v2;



};

#endif
