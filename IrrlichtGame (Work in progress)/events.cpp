// events.cpp
#define _USE_MATH_DEFINES
#include <irrlicht.h>
#include <math.h>
#include <cmath>
#include <string>
#include <iostream>



#include "events.h"
#include <Keycodes.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;





/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
	:node(nullptr), button_pressed(false), locked(false),jumped(false), camera(nullptr), enemy_locked(nullptr)
{ 
	old_cam_pos = ic::vector3df(0,0,0);
	old_cam_rot = ic::vector3df(0, 0, 0);
	for (u32 i = 0; i < 255; ++i)
		KeyIsDown[i] = false;
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                     
 * Used for press button 1 time only (not like movement)
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
	if (event.KeyInput.PressedDown)
	{
		ic::vector3df position = node->getPosition();
		ic::vector3df rotation = node->getRotation();
		switch (event.KeyInput.Key)
		{
		case KEY_SPACE:
			if (jumped == false) {
				node->setMD2Animation(is::EMAT_JUMP);
				node->setAnimationSpeed(24);
				jumped = true;
			}
			break;
		case KEY_ESCAPE:
			exit(0);
		case KEY_LMENU: // Avance
			if (locked) {
				locked = false;
				enemy_locked->setName("  ");
				enemy_locked = nullptr;
			}
			else
				locked = true;
			break;
		default:;
		}
	}

	return false;
}


/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse(const SEvent &event)
{
  switch(event.MouseInput.Event)
  {
    case EMIE_LMOUSE_PRESSED_DOWN:
      button_pressed = true;
      old_x = event.MouseInput.X;
      old_y = event.MouseInput.Y;
      break;
    case EMIE_LMOUSE_LEFT_UP:
      button_pressed = false;
      break;
    case EMIE_MOUSE_MOVED:
      if (button_pressed)
      {
        old_x = event.MouseInput.X;
        old_y = event.MouseInput.Y;
      }
      break;
    case EMIE_MOUSE_WHEEL:
      break;
    default:
      ;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
  switch (event.EventType)
  {
  case EET_KEY_INPUT_EVENT: {
	  bool un=keyboard(event);
	  KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	  jump_animation();
	  return false;}
    case EET_MOUSE_INPUT_EVENT:
      return mouse(event);
    default:;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::is_mouse_pressed                                        *
\**************************************************************************/
bool EventReceiver::is_mouse_pressed(int &x, int &y)
{
  if (button_pressed)
  {
    x = old_x;
    y = old_y;
    return true;
  }
  return false;
}

bool EventReceiver::is_KeyDown(irr::EKEY_CODE keyCode)
{
	return KeyIsDown[keyCode];
}

bool EventReceiver::is_locked()
{
  if (locked)
  {
    return true;
  }
  return false;
}

void EventReceiver::set_node_and_camera(irr::scene::IAnimatedMeshSceneNode *n, irr::scene::ISceneNode *c)
{
	node = n;
	camera = c;
	old_cam_pos = c->getAbsolutePosition();
	old_cam_rot = c->getRotation();
}

void EventReceiver::set_enemy_locked(irr::scene::ISceneNode *e)
{
	enemy_locked = e;
}

float * EventReceiver::smooth_camera(irr::ILogger *logger) {
	ic::vector3df diff_translation = old_cam_pos - camera->getAbsolutePosition();
	float diff_rotation = old_cam_rot.Y - camera->getRotation().Y;
	float length_translation = sqrt(pow(diff_translation.X, 2) + pow(diff_translation.Y, 2) + pow(diff_translation.Z, 2));
	float diffvector[5] = { diff_rotation, length_translation, diff_translation.X,diff_translation.Y,diff_translation.Z };
	//logger->log(std::string(std::to_string(old_cam_rot.Y) + " ii iii  " + std::to_string(camera->getRotation().Y)).c_str());
	old_cam_pos = camera->getAbsolutePosition();
	old_cam_rot = camera->getRotation();
	if (sqrt(pow(diff_rotation,2)) > 30.0f || length_translation > 100.0f) {
		camera->setRotation(ic::vector3df(old_cam_rot.X, old_cam_rot.Y + diff_rotation / 60, old_cam_rot.Z));
		return diffvector;
	}
	else
		return NULL;
}

void EventReceiver::jump_animation(){
	if (jumped) {
		ic::vector3df position = node->getAbsolutePosition();
		ic::vector3df position_cam = camera->getPosition();
		position.Y += 30;
		position_cam.Y -= 30;

		if (node->getFrameNr() >= node->getEndFrame() - 4)
		{
			node->setMD2Animation(is::EMAT_STAND);
			jumped = false;
			//is_jumping = false;
		}
		if (node->getFrameNr() < node->getEndFrame()) {
		}
		if (node->getFrameNr() < node->getEndFrame() - (node->getEndFrame() / node->getFrameNr()) / 1.5f) {
		}
		node->setPosition(position);
		camera->setPosition(position_cam);
	}
}
bool EventReceiver::is_jumping(){
	if (jumped)
		return true;
	else
		return false;
}
