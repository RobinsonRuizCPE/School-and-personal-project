

#include <GL/glew.h>

#include "scene.hpp"
#include "../../lib/opengl/glutils.hpp"

#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"
#include "../../lib/common/error_handling.hpp"
#include "../../lib/opengl/line_opengl.hpp"


#include <cmath>
#include <string>
#include <sstream>



using namespace cpe;



static cpe::mesh build_ground(float const L,float const h);
static cpe::mesh build_sphere(float radius,vec3 center);


void scene::load_scene()
{
    time_integration.restart();
    delta_t=0.02f;


    //*****************************************//
    // Preload default structure               //
    //*****************************************//
    texture_default = load_texture_file("data/white.jpg");
    shader_mesh     = read_shader("shaders/shader_mesh.vert",
                                  "shaders/shader_mesh.frag");
    shader_sphere     = read_shader("shaders/shader_sphere.vert",
                                  "shaders/shader_sphere.frag");


    texture_ground = load_texture_file("data/wood_texture.png");

    //*****************************************//
    // Build ground
    //*****************************************//
    mesh_ground = build_ground(1.0f , -2.5f);
    mesh_ground.fill_empty_field_by_default();
    mesh_ground_opengl.fill_vbo(mesh_ground);

    //*****************************************//
    // Sphere
    //*****************************************//
    mesh_sphere = build_sphere(0.05f , {0.0f,0.0f,0.0f});
    mesh_sphere.fill_empty_field_by_default();
    mesh_sphere_opengl.fill_vbo(mesh_sphere);

    //*****************************************//
    // Init spring
    //*****************************************//

    p0 = vec3(0.0f,0.0f,0.5f);
    p1 = vec3(0.0f,0.0f,0.4f);
    v1 = vec3(1.0f,1.0f,3.0f);
    p2 = vec3(0.0f,0.0f,0.3f);
    v2 = vec3(0.0f,0.0f,0.0f);
    L10_rest = 0.2f;


}



void scene::draw_scene()
{

    setup_shader_mesh(shader_mesh);

    // draw the ground
    glBindTexture(GL_TEXTURE_2D,texture_ground);                                                       PRINT_OPENGL_ERROR();
    mesh_ground_opengl.draw();
    glBindTexture(GL_TEXTURE_2D,texture_default);                                                      PRINT_OPENGL_ERROR();


    static float const mu = 0.2f;
    static float K = 20.0f;
    static vec3 const g (0.0f,0.0f,-9.81f);
    if( time_integration.elapsed() > 5 )
    {

        vec3 const u0 = p0-p1;
        float const L10 = norm(u0);

        vec3 const f10 = K * (L10-L10_rest) * u0/L10;


        //**********************
        // Second spring
        //**********************


        vec3 const u1 = p1-p2;
        float const L21 = norm(u1);

        vec3 const f21 = K * (L21 - L10_rest) * u1/L21;





        v1 = (1-mu*delta_t)*v1 + delta_t*(f10-f21+g);
        v2 = (1-mu*delta_t)*v2 + delta_t*(f21+g);

        p2 =                p2 + delta_t*v2;
        p1 =                p1 + delta_t*v1;


        time_integration.restart();
    }


    setup_shader_mesh(shader_sphere);

    //draw p0
    glUniform3f(get_uni_loc(shader_sphere,"translation") , p0.x(),p0.y(),p0.z());
    mesh_sphere_opengl.draw();

    //draw p1
    glUniform3f(get_uni_loc(shader_sphere,"translation") , p1.x(),p1.y(),p1.z());
    mesh_sphere_opengl.draw();

    //draw p2
    glUniform3f(get_uni_loc(shader_sphere,"translation") , p2.x(),p2.y(),p2.z());
    mesh_sphere_opengl.draw();


    // draw p0-p1
    line_opengl line;
    line.init();
    glUseProgram(line.shader_id());
    camera_matrices const& cam=pwidget->camera();
    glUniformMatrix4fv(get_uni_loc(line.shader_id(),"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(line.shader_id(),"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    line.draw(p0,p1);





    // draw p1-p2
    /*
    line_opengl line;
    line.init();
    glUseProgram(line.shader_id());
    camera_matrices const& cam=pwidget->camera();
    glUniformMatrix4fv(get_uni_loc(line.shader_id(),"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(line.shader_id(),"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
   */
    line.draw(p1,p2);


}


void scene::setup_shader_mesh(GLuint const shader_id)
{
    //Setup uniform parameters
    glUseProgram(shader_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_id,"normal_matrix"),1,false,cam.normal.pointer());           PRINT_OPENGL_ERROR();

    //load white texture
    glBindTexture(GL_TEXTURE_2D,texture_default);                                                      PRINT_OPENGL_ERROR();

}


scene::scene()
    :shader_mesh(0)
{}


GLuint scene::load_texture_file(std::string const& filename)
{
    return pwidget->load_texture_file(filename);
}

void scene::set_widget(myWidgetGL* widget_param)
{
    pwidget=widget_param;
}

static cpe::mesh build_ground(float const L,float const h)
{
    mesh m;
    m.add_vertex({ -L/2.0f, -L , h });
    m.add_vertex({ -L/2.0f,  L , h });
    m.add_vertex({ 1.5f*L,  L , h });
    m.add_vertex({ 1.5f*L, -L , h });

    m.add_texture_coord({  0.0f ,  0.0f });
    m.add_texture_coord({  0.0f ,  1.0f });
    m.add_texture_coord({  1.0f ,  1.0f });
    m.add_texture_coord({  1.0f ,  0.0f });

    m.add_triangle_index({0,2,1});
    m.add_triangle_index({0,3,2});


    return m;
}

static cpe::mesh build_sphere(float radius,vec3 center)
{
    mesh m;
    m.load("data/sphere.off");
    m.transform_apply_scale(radius);
    m.transform_apply_translation(center);
    return m;
}

