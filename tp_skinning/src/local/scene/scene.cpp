

#include <GL/glew.h>

#include "scene.hpp"
#include "../../lib/opengl/glutils.hpp"

#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"

#include "../interface/myWidgetGL.hpp"

#include <cmath>
#include <QTime>
#include <Qt>

#include <string>
#include <sstream>
#include "../../lib/mesh/mesh_io.hpp"


using namespace cpe;
QTime t;
int frame_number = 0;

static cpe::mesh build_ground(float const L,float const h)
{
    mesh m;
    m.add_vertex(vec3(-L, h,-L));
    m.add_vertex(vec3(-L, h, L));
    m.add_vertex(vec3( L, h, L));
    m.add_vertex(vec3( L, h,-L));

    m.add_triangle_index({0,2,1});
    m.add_triangle_index({0,3,2});

    m.fill_color(vec3(0.8,0.9,0.8));



    return m;
}
 static cpe::mesh_skinned build_cylinder(float const r, float const h,int const Nc,int const Nh)
 {
     mesh_skinned m;
     float const angle = 2*M_PI/Nc;
     float const hpas = h/Nh;

     //Creation vertices
     //m.add_vertex(vec3(0,0 ,0));//for fan filling
     for (int k=0;k<Nh;k++)
     { 
        for(int kc=0;kc<Nc;kc++)
        {
            m.add_vertex(vec3(cos(kc*angle)*r,sin(kc*angle)*r ,k*hpas));
        }

     }
     //m.add_vertex(vec3(0,0 ,h)); for fan filling

     //Add weight to vertices
     vertex_weight_parameter W;
     skinning_weight w0;
     skinning_weight w1;
     w0.joint_id = 0;
     w0.weight = 1;
     w1.joint_id = 1;
     w1.weight = 0;


     for (int k=0;k<Nh;k++)
     {
        for(int kc=0;kc<Nc;kc++)
        {

            float weight = (float)k/(float)Nh;
            w0.weight = 1.0-weight;
            w1.weight = weight;
            W[0] = w0;
            W[1] = w1;
            normalized(W);
            m.add_vertex_weight(W);

            /*
            //First weight method
            if(2*k <= Nh)
            {
                W[0] = w0;
                W[1] = w1;
                normalized(W);
                m.add_vertex_weight(W);
            }
            else
            {
                W[0] = w0;
                W[1] = w1;
                W[1].weight = 1;
                W[0].weight = 0;
                normalized(W);
                m.add_vertex_weight(W);
            }
            */

        }

     }

     //Creation tri
     for (int k=0;k<Nh-1;k++)
     {
        for(int kc=0;kc<Nc;kc++)
        {
             /* //TO COMPLETE: Case fill cylinder extremities with fan triangles
             if(k == 0)
             {
                m.add_triangle_index({0,kc+1,kc+2%Nc});
             }
             else if (k == Nh)
             {
                 m.add_triangle_index({Nc*Nh+2,Nc*Nh+1-kc,Nc*Nh-kc});
             }
             else
             {
             }
             */
             m.add_triangle_index({k*Nc+kc,k*Nc+(kc+1)%Nc,(k+1)*Nc+kc});
             m.add_triangle_index({(k+1)*Nc+kc,k*Nc+(kc+1)%Nc,(k+1)*Nc+(kc+1)%Nc});
        }
     }
     m.fill_color(vec3(0.8,0.2,0.3));
    return m;
 }

static cpe::skeleton_geometry init_bind_pose(float const h)
{
   skeleton_geometry bind_pose;
   bind_pose.push_back(skeleton_joint(vec3(0.f,0.f,0.f),quaternion(0.f,0.f,0.f,1.f)));//centre base cylindre en (0,0,0) global et aligné avec repere global
   bind_pose.push_back(skeleton_joint(vec3(0.f,0.f,h/2.f),quaternion(0.f,0.f,0.f,1.f)));
   bind_pose.push_back(skeleton_joint(vec3(0.f,0.f,h/2.f),quaternion(0.f,0.f,0.f,1.f)));
   return bind_pose;

}

static cpe::skeleton_parent_id init_parent_id()
{
    skeleton_parent_id parent_id;
    parent_id.push_back(-1);
    parent_id.push_back(0);
    parent_id.push_back(1);
    return parent_id;
}

static cpe::skeleton_animation animate(skeleton_geometry const& bind_pose)
{
    skeleton_animation animation;
    skeleton_geometry frame = bind_pose;
    quaternion pas;
    pas.set_axis_angle(vec3(1.f,0.f,0.f),M_PI/6.f);

    animation.push_back(frame);
    frame[1].orientation = frame[1].orientation * pas;
    animation.push_back(frame);
    frame[1].orientation = frame[1].orientation * pas;
    animation.push_back(frame);
    frame[1].orientation = frame[1].orientation * pas;
    animation.push_back(frame);

    return animation;
}

void scene::load_scene()
{


    //*****************************************//
    // Preload default structure               //
    //*****************************************//
    texture_default = load_texture_file("data/cat.png");
    shader_mesh     = read_shader("shaders/shader_mesh.vert",
                                  "shaders/shader_mesh.frag");           PRINT_OPENGL_ERROR();
    shader_skeleton = read_shader("shaders/shader_skeleton.vert",
                                "shaders/shader_skeleton.frag");       PRINT_OPENGL_ERROR();


    //*****************************************//
    // Build ground
    //*****************************************//
    mesh_ground = build_ground(100.0f , -25.0f);
    mesh_ground.fill_empty_field_by_default();
    mesh_ground_opengl.fill_vbo(mesh_ground);


    //*****************************************//
    // Build cylinder
    //*****************************************//
    float const length = 50.0f;
    float const radius = 4.0f;
    mesh_cylinder = build_cylinder(radius,length,40,40);
    mesh_cylinder.fill_empty_field_by_default();
    mesh_cylinder_opengl.fill_vbo(mesh_cylinder);

    sk_cylinder_parent_id = init_parent_id();
    sk_cylinder_bind_pose = init_bind_pose(length);
    sk_cylinder_animation = animate(sk_cylinder_bind_pose);

    //*****************************************//
    // Build cat
    //*****************************************//
    mesh_cat.load("data/cat.obj");
    texture_cat = load_texture_file("data/cat.png");
    mesh_cat.fill_empty_field_by_default();
    //mesh_cat.
    mesh_cat_opengl.fill_vbo(mesh_cat);

    sk_cat_parent_id.load("data/cat_bind_pose.skeleton");
    sk_cat_bind_pose.load("data/cat_bind_pose.skeleton");
    sk_cat_animation.load("data/cat.animations",sk_cat_parent_id.size());

}



void scene::draw_scene()
{
    int time_frame = 10; //ms
    setup_shader_skeleton(shader_skeleton);
    //Here we can draw skeletons as 3D segments
    if(t.elapsed() > time_frame )
    {
            t.start();
            frame_number += 1;
            if(frame_number >= sk_cat_animation.size())
            {
                frame_number=0;
            }
    }
    setup_shader_mesh(shader_mesh);

    mesh_ground_opengl.draw();


    //**************************************
    // Cylinder
    //**************************************

    /*
    if(t.elapsed() > time_frame )
    {
            t.start();
            frame_number += 1;
            if(frame_number > 3)
            {
                frame_number=0;
            }
    }

    float alpha = (float)t.elapsed()/(float)time_frame;
    skeleton_geometry sk_cylinder_local_anim = sk_cylinder_animation(frame_number,alpha);
    skeleton_geometry const sk_cylinder_global =
    local_to_global(sk_cylinder_local_anim,sk_cylinder_parent_id);
    std::vector<vec3> const sk_cylinder_bones =
    extract_bones(sk_cylinder_global,sk_cylinder_parent_id);
    draw_skeleton(sk_cylinder_bones);


    skeleton_geometry const sk_cylinder_inverse_bind_pose =
    inversed(sk_cylinder_bind_pose);
    skeleton_geometry const sk_cylinder_binded =
    multiply(sk_cylinder_global,sk_cylinder_inverse_bind_pose);
    mesh_cylinder.apply_skinning(sk_cylinder_binded);
    mesh_cylinder.fill_normal();
    mesh_cylinder_opengl.update_vbo_vertex(mesh_cylinder);
    mesh_cylinder_opengl.update_vbo_normal(mesh_cylinder);
    mesh_cylinder_opengl.draw();
    */


    //**************************************
    // Cat
    //**************************************


    float alpha = (float)t.elapsed()/(float)time_frame;
    skeleton_geometry sk_cat_local_anim = sk_cat_animation(frame_number,alpha);

    skeleton_geometry const sk_cat_global =
    local_to_global(sk_cat_local_anim,sk_cat_parent_id);

    std::vector<vec3> const sk_cat_bones =
    extract_bones(sk_cat_global,sk_cat_parent_id);

    draw_skeleton(sk_cat_bones);


    skeleton_geometry const sk_cat_inverse_bind_pose =
    inversed(sk_cat_bind_pose);
    skeleton_geometry const sk_cat_binded =
    multiply(sk_cat_global,sk_cat_inverse_bind_pose);
    mesh_cat.apply_skinning(sk_cat_binded);
    mesh_cylinder.fill_normal();
    mesh_cat_opengl.update_vbo_vertex(mesh_cat);
    mesh_cat_opengl.update_vbo_normal(mesh_cat);
    mesh_cat_opengl.draw();





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
    glLineWidth(1.0f);                                                                                 PRINT_OPENGL_ERROR();

}

void scene::setup_shader_skeleton(GLuint const shader_id)
{
    //Setup uniform parameters
    glUseProgram(shader_id);                                                                           PRINT_OPENGL_ERROR();

    //Get cameras parameters (modelview,projection,normal).
    camera_matrices const& cam=pwidget->camera();

    //Set Uniform data to GPU
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_modelview"),1,false,cam.modelview.pointer());     PRINT_OPENGL_ERROR();
    glUniformMatrix4fv(get_uni_loc(shader_id,"camera_projection"),1,false,cam.projection.pointer());   PRINT_OPENGL_ERROR();
    glUniform3f(get_uni_loc(shader_id,"color") , 0.0f,0.0f,0.0f);                                      PRINT_OPENGL_ERROR();

    //size of the lines
    glLineWidth(3.0f);                                                                                 PRINT_OPENGL_ERROR();
}

void scene::draw_skeleton(std::vector<vec3> const& positions) const
{
    // Create temporary a VBO to store data
    GLuint vbo_skeleton=0;
    glGenBuffers(1,&vbo_skeleton);                                                                     PRINT_OPENGL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER,vbo_skeleton);                                                        PRINT_OPENGL_ERROR();

    // Update data on the GPU
    glBufferData(GL_ARRAY_BUFFER , sizeof(vec3)*positions.size() , &positions[0] , GL_STATIC_DRAW);    PRINT_OPENGL_ERROR();

    // Draw data
    glEnableClientState(GL_VERTEX_ARRAY);                                                              PRINT_OPENGL_ERROR();
    glVertexPointer(3, GL_FLOAT, 0, 0);                                                                PRINT_OPENGL_ERROR();
    glDrawArrays(GL_LINES,0,positions.size());                                                         PRINT_OPENGL_ERROR();

    // Delete temporary VBO
    glDeleteBuffers(1,&vbo_skeleton);                                                                  PRINT_OPENGL_ERROR();
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


