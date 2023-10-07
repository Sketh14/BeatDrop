#ifndef TEST_H
#define TEST_H

//#define KEY_INPUT
#define TOUCH_INPUT

#ifdef KEY_INPUT
#include <godot_cpp/classes/input_event_key.hpp>
#endif

#ifdef TOUCH_INPUT
#include <godot_cpp/classes/input_event_screen_touch.hpp>
#include <godot_cpp/classes/input_event_screen_drag.hpp>
#endif


#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/input_event.hpp>

#include "DropsSpawner.h"
#include "DropsController.h"

using namespace godot;

class DropsSpawner;
class DropsController;
class Test : public Node2D
{
	GDCLASS(Test, Node2D);

public:
	void _ready();
	void _process(double delta);
	void _input(const Ref<InputEvent>& event);
	
	void SetTestTex(Ref<Texture2D> tex);
	Ref<Texture2D> GetTestTex();

	void SetMoveForceMult(float speed);
	float GetMoveForceMult();

	void SetPlayMode(bool toggleVal);
	bool GetPlayMode();

	//void SetPlayerRB(RigidBody2D rigidBody);				//
	//RigidBody2D GetPlayerRB();

protected : 
	static void _bind_methods();

private:
	Ref<Texture2D> _testTex;
	Vector2 center, moveForce;			//, pos
	bool playMode = false, moving = false;
	bool movingLeft = false, movingRight = false;			//, movingUp, movingDown
	float moveForceMult = 10.f, moveMultX = 1.0f;		//, moveMultY = 1.0f

#ifdef TOUCH_INPUT
	float deltaX;
	bool rayCastHitPlayer;
#endif

	RigidBody2D* playerRb;
	Area2D* playerArea;
	DropsSpawner* dropSpawner;
	Node2D* playerNode;

	AudioStreamPlayer2D* playerSFX[4];

	void OnBodyEntered(Variant body);
	Test();
	~Test();
};

#endif // !TEST_H
