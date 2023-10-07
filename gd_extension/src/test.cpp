#include "test.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

Test::Test()
{
	moveForce = center = Vector2(0, 0);			//, pos
	playMode = moving = false;
	movingLeft = movingRight = rayCastHitPlayer = false;			//, movingUp, movingDown
	moveForceMult = 10.f, moveMultX = 1.0f;		//, moveMultY = 1.0f
	deltaX = 0.0f;

	playerRb = NULL, playerArea = NULL, playerNode = NULL, dropSpawner = NULL;

	for (int i = 0; i < 4; i++)
		playerSFX[i] = NULL;
}

Test::~Test() { }

void Test::_ready()
{
	UtilityFunctions::print("Hello World from GDExtension");

	//pos = Vector2(0, 100);
	moveForce = Vector2(0, 0);

	if (Engine::get_singleton()->is_editor_hint())
		return;

	playerRb = get_node<RigidBody2D>("/root/Main/Player");
	playerArea = get_node<Area2D>("/root/Main/Player/Area2D");
	dropSpawner = get_node<DropsSpawner>("/root/Main/WaterDropsContainer/DropsSpawner");

	if (playerArea == NULL)
		UtilityFunctions::print("Player Rb not found");
	else	//This is because the Editor will try to access it, while loading up.
	{
		playerArea->connect("body_entered", Callable(this, "OnBodyEntered"));
		playMode = true;
	}

	//for (int i = 1; i < 5; i++)
	{
		//std::string path = "/root/Main/Player/SFX_Manager/SFX" + i;
		//NodePath nodePath = new NodePath()
		playerSFX[0] = get_node<AudioStreamPlayer2D>("/root/Main/Player/SFX_Manager/SFX1");
		playerSFX[1] = get_node<AudioStreamPlayer2D>("/root/Main/Player/SFX_Manager/SFX2");
		playerSFX[2] = get_node<AudioStreamPlayer2D>("/root/Main/Player/SFX_Manager/SFX3");
		playerSFX[3] = get_node<AudioStreamPlayer2D>("/root/Main/Player/SFX_Manager/SFX4");
	}

	playerNode = get_node<Node2D>("/root/Main/Player");

	//get_node<Test>("/root/Main/Player/Test");	
	//print_tree();
}

void Test::OnBodyEntered(Variant body)
{
	UtilityFunctions::print("Collision Detected");

	//unsigned short ID = 0;
	Node2D* dropCollected = Object::cast_to<Node2D>(body);
	if (dropCollected != NULL)
	{
		DropsController* controllerNode = Object::cast_to<DropsController>(dropCollected->get_child(0));
		if (controllerNode != NULL)
		{
			dropSpawner->DisableDrop(controllerNode->dropID, dropCollected);

			/*for (int i = 0; i < 4; i++)
				if (!playerSFX[i]->is_playing())
					playerSFX[i]->play();*/

			playerSFX[0]->play();
			//Not Any of this below
			//controllerNode->reparent(NULL);
			//dropCollected->call_deferred("queue_free");
			//dropCollected->set_process(false);				//<=========== not this
		}
	}

	//dropSpawner->dropsSpawned--;
}

void Test::_process(double delta)
{
	if (!playMode)
		return;

	if (moving)
	{
		//pos.x = pos.x + moveSpeed * moveMultX * delta;
		//pos.y = pos.y + moveSpeed * moveMultY * delta;
		//set_position(pos);

		playerRb->apply_impulse(moveForce * moveForceMult, Vector2(0, 0));
		//UtilityFunctions::print("Player Pos: ", playerNode->get_position().x);
	}

	/*if (pos.x > 720.0f)
		pos.x = 0.0f;
	else if (pos.x < 0.0f)
		pos.x = 720.0f;
	else if (pos.y > 480.0f)
		pos.y = 0.0f;
	else if (pos.y < 0)
		pos.y = 480.0f;*/
}

/*void Test::_input(const Ref<InputEvent>& event)
{
	if (event != nullptr)
	{
		bool isInputEventKey = event->is_class("InputEventKey");

		if (isInputEventKey)
		{
			Ref<InputEventKey> inputKey = static_cast<Ref<InputEventKey>>(event);
			if (inputKey->is_pressed())
			{
				moving = true;
				UtilityFunctions::print("Key Pressed");

				switch (inputKey->get_keycode())
				{
				case KEY_A:
				case KEY_LEFT:
					movingLeft = true;
					moveMultX = -2.0f;
					break;

				case KEY_D:
				case KEY_RIGHT:
					movingRight = true;
					moveMultX = 2.0f;
					break;

				case KEY_W:
				case KEY_UP:
					movingUp = true;
					moveMultY = -2.0f;
					break;

				case KEY_S:
				case KEY_DOWN:
					movingDown = true;
					moveMultY = 2.0f;
					break;

				default:
					UtilityFunctions::print("Key Pressed");
					break;
				}
			}
			else if (inputKey->is_released())
			{
				UtilityFunctions::print("Key Released");

				switch (inputKey->get_keycode())
				{
				case KEY_A:
				case KEY_LEFT:
					movingLeft = false;
					break;

				case KEY_D:
				case KEY_RIGHT:
					movingRight = false;
					break;

				case KEY_W:
				case KEY_UP:
					movingUp = false;
					break;

				case KEY_S:
				case KEY_DOWN:
					movingDown = false;
					break;

				default:
					UtilityFunctions::print("Key Pressed");
					break;
				}
				
				if (!movingLeft && movingRight)
					moveMultX = 2.0f;
				else if (!movingRight && movingLeft)
					moveMultX = -2.0f;
				else
					moveMultX = 0.0f;

				if (!movingUp && movingDown)
					moveMultY = 2.0f;
				else if (!movingDown && movingUp)
					moveMultY = -2.0f;
				else
					moveMultY = 0.0f;
			}
		}
	}
}*/

void Test::_input(const Ref<InputEvent>& event)
{
	if (event != nullptr)
	{

#ifdef TOUCH_INPUT
		bool isTouchEvent = event->is_class("InputEventScreenTouch");
		if (isTouchEvent)
		{
			Ref<InputEventScreenTouch> inputTouch = static_cast<Ref<InputEventScreenTouch>>(event);
			if (inputTouch->is_pressed())
			{
				Vector2 touchPos = inputTouch->get_position();
				deltaX = touchPos.x - get_position().x;

				//RayCast code to check if tapped on player
			}
			else if (inputTouch->is_released())
			{
				Vector2 touchPos = inputTouch->get_position();
				float newPosX = 
			}
		}
#endif

#ifdef KEY_INPUT
		bool isInputEventKey = event->is_class("InputEventKey");
		if (isInputEventKey)
		{
			Ref<InputEventKey> inputKey = static_cast<Ref<InputEventKey>>(event);
			if (inputKey->is_pressed())
			{
				moving = true;
				//UtilityFunctions::print("Key Pressed");

				switch (inputKey->get_keycode())
				{
				case KEY_A:
				case KEY_LEFT:
					movingLeft = true;
					//moveMultX = -2.0f;
					moveForce.x = -2.0f;
					break;

				case KEY_D:
				case KEY_RIGHT:
					movingRight = true;
					//moveMultX = 2.0f;
					moveForce.x = 2.0f;
					break;

				default:
					UtilityFunctions::print("Key Pressed");
					break;
				}
			}
			else if (inputKey->is_released())
			{
				//UtilityFunctions::print("Key Released");

				switch (inputKey->get_keycode())
				{
				case KEY_A:
				case KEY_LEFT:
					movingLeft = false;
					break;

				case KEY_D:
				case KEY_RIGHT:
					movingRight = false;
					break;

				default:
					UtilityFunctions::print("Key Pressed");
					break;
				}
				
				if (!movingLeft && movingRight)
				{
					//moveMultX = 2.0f;
					moveForce.x = 2.0f;
				}
				else if (!movingRight && movingLeft)
				{
					//moveMultX = -2.0f;
					moveForce.x = -2.0f;
				}
				else
				{
					//moveMultX = 0.0f;
					moveForce.x = 0.0f;
					moving = false;
				}
			}
		}
#endif
	}
}

#pragma region Properties
float Test::GetMoveForceMult() { return moveForceMult; }
void Test::SetMoveForceMult(float speed) { moveForceMult = speed; }

Ref<Texture2D> Test::GetTestTex() { return _testTex; }
void Test::SetTestTex(Ref<Texture2D> tex) { _testTex = tex; }

bool Test::GetPlayMode() { return playMode; }
void Test::SetPlayMode(bool toggleVal) { playMode = toggleVal;}

//RigidBody2D Test::GetPlayerRB() { return playerRb; }
//void Test::SetPlayerRB(RigidBody2D rigidBody) { get_node<RigidBody2D>("PlayerRB_2D"); }			//RigidBody2D rigidBody
#pragma endregion

void Test::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("SetMoveForceMult" , "speed"), &SetMoveForceMult);
	ClassDB::bind_method(D_METHOD("GetMoveForceMult"), &GetMoveForceMult);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "moveForceMult"), "SetMoveForceMult", "GetMoveForceMult");

	ClassDB::bind_method(D_METHOD("SetTestTex", "tex"), &SetTestTex);
	ClassDB::bind_method(D_METHOD("GetTestTex"), &GetTestTex);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_testTex", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "SetTestTex", "GetTestTex");

	//ClassDB::bind_method(D_METHOD("SetPlayerRB", "rigidBody"), &SetPlayerRB);
	////ClassDB::bind_method(D_METHOD("SetPlayerRB"), &SetPlayerRB);
	//ClassDB::bind_method(D_METHOD("GetPlayerRB"), &GetPlayerRB);
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "playerRB", PROPERTY_HINT_RESOURCE_TYPE, "RigidBody2D"), "SetPlayerRB", "GetPlayerRB");

	ClassDB::bind_method(D_METHOD("SetPlayMode", "toggleVal"), &SetPlayMode);
	ClassDB::bind_method(D_METHOD("GetPlayMode"), &GetPlayMode);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enablePlayMode"), "SetPlayMode", "GetPlayMode");

	ClassDB::bind_method(D_METHOD("OnBodyEntered", "body"), &OnBodyEntered);
	
}