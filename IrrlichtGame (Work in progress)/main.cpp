// main.cpp

#include <iostream>
#include <string>
#include <math.h>

#define _USE_MATH_DEFINES
#include <irrlicht.h>

#include "events.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
int identity = 1;
const int ENEMY_ID = 42;
const int BILL = 999;
is::IBillboardSceneNode *billboard;
int lastFPS = -1;
int enemy_lock_id = 999;


ic::vector3df getYXangle(core::vector3df r)
{
	ic::vector3df angle;
	angle.Y = atan2(r.X, r.Z);
	angle.Y *= (180 / M_PI);
	if (angle.Y < 0) angle.Y += 360;
	if (angle.Y >= 360) angle.Y -= 360;
	float z1;
	z1 = sqrt(r.X * r.X + r.Z * r.Z);
	angle.X = atan2(z1, r.Y);
	angle.X *= (180 / M_PI);
	angle.X -= 90;
	if (angle.X < 0) angle.X += 360;
	if (angle.X >= 360) angle.X -= 360;
	return angle;
}

int find_closest_target(ic::vector3df mainchar_position, is::ISceneManager *smgr){
	float previous_dist = 500;
	int retained_id = 0;
	for (int k = 1; k < identity; k++) {
		is::ISceneNode* mechant = smgr->getSceneNodeFromId(k, NULL);
		ic::vector3df mechant_position = mechant->getAbsolutePosition();
		float distance =sqrt((mechant_position.X - mainchar_position.X)*(mechant_position.X - mainchar_position.X) + (mechant_position.Y - mainchar_position.Y)*(mechant_position.Y - mainchar_position.Y) + (mechant_position.Z - mainchar_position.Z)*(mechant_position.Z - mainchar_position.Z));
		if (distance <= previous_dist) {
			retained_id = mechant->getID();
			previous_dist = distance;
		}
	}
	return retained_id;
}


void lockon(is::ISceneNode* mechant, is::ISceneManager *smgr, iv::IVideoDriver  *driver) {
	if (billboard != nullptr) {
		billboard->remove();
		billboard = nullptr;
	}
	if (mechant->getID() == enemy_lock_id) {
		}
	else {
		ic::vector3df position = mechant->getAbsolutePosition();
		position.Z = position.Z; // lock on circle 
		billboard = smgr->addBillboardSceneNode(nullptr,
			ic::dimension2d<f32>(25, 25),
			position);
		billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		billboard->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		billboard->setMaterialTexture(0, driver->getTexture("data/cercle.png"));
		mechant->setName("TARGET");


	}
}



int main()
{
  // Le gestionnaire d'événements
  EventReceiver receiver;
  iv::ITexture *base_texture;
  iv::ITexture *wounded_texture;
  is::ISceneNode *map;
  is::ISceneNode *clone_shell;

  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(1280, 900),
                                        16, false, false, true, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *camera_player = device->getSceneManager();
  is::ISceneManager *smgr = device->getSceneManager();
  irr::ILogger *logger = device->getLogger();


  base_texture    = driver->getTexture("data/base.pcx");
  wounded_texture = driver->getTexture("data/red_texture.pcx");
  // Chargement de notre personnage
  is::IAnimatedMesh *mesh = camera_player->getMesh("data/tris.md2");

  // Création de plusieurs personnages
  is::IAnimatedMeshSceneNode *node;
  is::IAnimatedMeshSceneNode *enemies;
  is::ISceneNodeAnimator *anim;
  is::ITriangleSelector *selector;
  is::ITriangleSelector *selector_char;

  // some info 
  ic::vector3df radius; //Bounding box radius
  ic::aabbox3d<f32> box;

  // Terrain
  device->getFileSystem()->addFileArchive("data/Q3shw21.pk3");
  is::IAnimatedMesh *terrain = smgr->getMesh("q3shw21.bsp");
  map = smgr->addOctreeSceneNode(terrain->getMesh(0), nullptr, -1, 1024);
  selector = smgr->createOctreeTriangleSelector(terrain->getMesh(0), map);
  map->setPosition(core::vector3df(-40, -300, 0));
  map->setTriangleSelector(selector);

    // main char
  node = smgr->addAnimatedMeshSceneNode(mesh);
  node->setMaterialFlag(iv::EMF_LIGHTING, false);
  node->setMD2Animation(is::EMAT_SALUTE);
  node->setMaterialTexture(0, base_texture);
  node->setPosition(ic::vector3df(10, 0, -100));
  node->setRotation(ic::vector3df(0, 0, 0));
  node->setID(10);
  box = node->getBoundingBox();
  radius = box.MaxEdge - box.getCenter();
  anim = smgr->createCollisionResponseAnimator(selector,node,radius, ic::vector3df(0, -8, 0), ic::vector3df(0, 0, 0));
  node->addAnimator(anim);
  node->setDebugDataVisible(is::EDS_BBOX_ALL);
  selector_char = smgr->createTriangleSelector(node);
  node->setTriangleSelector(selector_char);
//   selector->drop();
//   selector = smgr->createTriangleSelector(node);
//   node->setTriangleSelector(selector);

  for (int i = -100; i <= 100; i += 150)
  {
	  enemies = smgr->addAnimatedMeshSceneNode(mesh);
	  enemies->setMaterialFlag(iv::EMF_LIGHTING, false);
	  enemies->setMD2Animation(is::EMAT_STAND);
	  enemies->setMaterialTexture(0, base_texture);
	  enemies->setPosition(ic::vector3df(i, 0, 0));
	  enemies->setRotation(ic::vector3df(0, 90, 0));
	  enemies->setID(identity);
	  identity = identity + 1;
	  box = enemies->getBoundingBox();
	  radius = box.MaxEdge - box.getCenter();
	  // Terrain collision
	  selector = smgr->createOctreeTriangleSelector(terrain->getMesh(0), map);
	  map->setTriangleSelector(selector);
	  anim = smgr->createCollisionResponseAnimator(selector, enemies, radius, ic::vector3df(0, -8, 0), ic::vector3df(0, 0, 0));
	  enemies->addAnimator(anim);
	  enemies->setDebugDataVisible(is::EDS_BBOX_ALL);

	  // player collision //NOT WORKING
  }
  for (int k = 1; k < identity; k++) {
	  is::ISceneNode* mechant = smgr->getSceneNodeFromId(k, NULL);
	anim = smgr->createCollisionResponseAnimator(selector_char,mechant,radius,ic::vector3df(0, 0, 0), ic::vector3df(0, 0, 0));  
	mechant->addAnimator(anim);
    selector->drop();    
  }
  bool is_jumping = false;
  bool runing = false;
  bool is_turning = false;
  // collision
  is::ISceneCollisionManager *collision_manager = camera_player->getSceneCollisionManager();

  camera_player->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -140), ic::vector3df(0, 5, 0));
  int enemy_lock_id = 999;
  int jump_status = 0; 
  smgr->getActiveCamera()->setParent(node);
  smgr->getActiveCamera()->setTarget(node->getPosition());

  smgr->getActiveCamera()->setPosition(ic::vector3df(0, 30, -60));
  receiver.set_node_and_camera(node,smgr->getActiveCamera());
  while (device->run())
  {
	  driver->beginScene(true, true, iv::SColor(0, 50, 100, 255));
	  // SET CAMERA POSITION BEHIND PLAYER //
	  is::ISceneNode *camera = camera_player->getActiveCamera();
	  camera_player->getActiveCamera()->setParent(node);
	  camera_player->getActiveCamera()->setTarget(node->getPosition());
	  if(is_turning==false)
		camera_player->getActiveCamera()->setPosition(ic::vector3df(0, 30, -60));

	  /// LOCK ON FEATURE ///
	  if (receiver.is_locked()) {
		  is::ISceneNode* mainchar = camera_player->getSceneNodeFromId(10, NULL);
		  ic::vector3df mainchar_position = mainchar->getAbsolutePosition();
		  is::ISceneNode* enemy = camera_player->getSceneNodeFromId(find_closest_target(mainchar_position, camera_player), NULL);
		  receiver.set_enemy_locked(enemy);


		  if (enemy !=nullptr && enemy->getID() == enemy_lock_id) {
			  ic::vector3df angle = getYXangle(enemy->getPosition() - mainchar->getPosition());
			  mainchar->setRotation(angle);
			  camera_player->getActiveCamera()->setParent(mainchar);
			  camera_player->getActiveCamera()->setTarget(mainchar->getAbsolutePosition());
			  camera_player->getActiveCamera()->bindTargetAndRotation(true);
			  //camera_player->getActiveCamera()->updateAbsolutePosition();
			  ic::vector3df mainchar_rotation = mainchar->getRotation();

		  }
		  else if (enemy_lock_id == 999) {
			  lockon(enemy, camera_player, driver);
			  enemy->setName("TARGET");
			  receiver.set_enemy_locked(enemy);
			  enemy_lock_id = enemy->getID();
			  billboard->setVisible(true);
		  }
		  else if (enemy->getID() != enemy_lock_id) {
			  enemy = smgr->getSceneNodeFromId(enemy_lock_id);
			  ic::vector3df angle = getYXangle(enemy->getPosition() - mainchar->getPosition());
			  mainchar->setRotation(angle);
			  camera_player->getActiveCamera()->setParent(mainchar);
			  camera_player->getActiveCamera()->setTarget(mainchar->getAbsolutePosition());
			  camera_player->getActiveCamera()->bindTargetAndRotation(true);
			  //camera_player->getActiveCamera()->updateAbsolutePosition();
			  ic::vector3df mainchar_rotation = mainchar->getRotation();
		  }

	  }

	  else {
		  if (billboard != nullptr) {
			  billboard->setVisible(false);
			  enemy_lock_id = 999;
		  }
		  //node->setRotation((ic::vector3df(0, -90, 0)));
	  }

	  // Dessin de la scène :
	  ic::vector3df char_position = node->getPosition();
	  ic::vector3df char_rotation = node->getRotation();

	  // Player movement //
	  float player_speed =1.3;
	  if (receiver.is_KeyDown(KEY_ESCAPE))
		  exit(0);
	  if (receiver.is_KeyDown(KEY_KEY_Z)) { // Avance
		  if (runing == false && is_jumping==false){
			  node->setMD2Animation(is::EMAT_RUN);
			 runing = true;
	  }
		  char_position.X += player_speed * sin(char_rotation.Y * M_PI / 180.0);
		  char_position.Z += player_speed * cos(char_rotation.Y * M_PI / 180.0);
	  }
	  else if (receiver.is_KeyDown(KEY_KEY_S)) { // Recule
		  if (runing == false && is_jumping == false) {
			  node->setMD2Animation(is::EMAT_RUN);
			  runing = true;
		  }
		  char_position.X += -player_speed * sin(char_rotation.Y * M_PI / 180.0);
		  char_position.Z += -player_speed * cos(char_rotation.Y * M_PI / 180.0);
	  }
	  else if (runing == true && is_jumping==false) {
		  node->setMD2Animation(is::EMAT_STAND);
		  runing = false;
	  }

	  if (receiver.is_KeyDown(KEY_KEY_D)) {// Tourne à droite
		  if (receiver.is_locked() == false) {
			  char_rotation.Y += 8;
		  }
		  char_position.X += player_speed * cos(char_rotation.Y * M_PI / 180.0);
		  char_position.Z += -player_speed * sin(char_rotation.Y * M_PI / 180.0);
	  }

	  if (receiver.is_KeyDown(KEY_KEY_Q)) {// Tourne à gauche

		  if (receiver.is_locked() == false  ) {
			  char_rotation.Y -= 8;
		  }
		  else {
			  char_position.X += -player_speed * cos(char_rotation.Y * M_PI / 180.0);
			  char_position.Z += player_speed * sin(char_rotation.Y * M_PI / 180.0);
		  }

	  }
	  if (receiver.is_KeyDown(KEY_TAB) && receiver.is_locked()) {
		  is::ISceneNode* enemy = smgr->getSceneNodeFromId(find_closest_target(node->getAbsolutePosition(), smgr), NULL);
		  if (enemy->getID() != enemy_lock_id) {
			  lockon(enemy, camera_player, driver);
			  enemy->setName("TARGET");
			  receiver.set_enemy_locked(enemy);
			  enemy_lock_id = enemy->getID();
			  billboard->setVisible(true);
		  }   
	  }
	  if (receiver.is_KeyDown(KEY_SPACE)) {

		  }
	  if(receiver.is_jumping())
		receiver.jump_animation();

	  


	  node->setPosition(char_position);
	  node->setRotation(char_rotation);
	  node->updateAbsolutePosition();	 
	 // receiver.smooth_camera(logger);
      camera->updateAbsolutePosition();


	  smgr->drawAll();
	  // 
	  driver->endScene();
	  //Get and show fps
	  if (driver->getFPS() != lastFPS)
	  {
		  lastFPS = driver->getFPS();
		  core::stringw tmp = L"(FPS: ";
		  tmp += lastFPS;
		  tmp += ")";
		  device->setWindowCaption(tmp.c_str());
	  }
	  lastFPS += 1;
  }
	  device->drop();
  
  return 0;
}
