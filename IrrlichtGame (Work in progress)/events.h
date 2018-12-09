// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>
#include <Keycodes.h>

#include <irrlicht.h>
irr::core::vector3df getYXangles(irr::core::vector3df r);
void compute_inputs();

class EventReceiver : public irr::IEventReceiver
{
  irr::scene::IAnimatedMeshSceneNode *node;
  irr::scene::ISceneNode *camera;
  irr::scene::ISceneNode *enemy_locked;
  irr::core::vector3df old_cam_pos;
  irr::core::vector3df old_cam_rot;

  bool button_pressed;
  int  old_x, old_y;
  bool locked = false;
  bool jumped = false;
  bool keyboard(const irr::SEvent &event);
  bool mouse(const irr::SEvent &event);

public:
  EventReceiver();
  bool OnEvent(const irr::SEvent &event);
  bool is_KeyDown(irr::EKEY_CODE keyCode);
  bool is_mouse_pressed(int &x, int &y);
  bool is_locked();
  bool is_jumping();
  void jump_animation();
  float * smooth_camera(irr::ILogger *logger);
  void set_node_and_camera(irr::scene::IAnimatedMeshSceneNode *n, irr::scene::ISceneNode *c);
  void set_enemy_locked(irr::scene::ISceneNode *e);
  bool KeyIsDown[255];
};

#endif
