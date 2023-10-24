#include "test.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

Test::Test()
{
	moveForce = center = Vector2(0, 0);			//, pos
	playMode = moving = false;
	movingLeft = movingRight = pressedOnPlayer = false;			//, movingUp, movingDown
	moveForceMult = 10.f, moveMultX = 1.0f;		//, moveMultY = 1.0f
	deltaX = deltaY = 0.0f;

	playerRb = NULL, playerArea = NULL, playerNode = NULL, dropSpawner = NULL;

	for (int i = 0; i < 4; i++)
		playerSFX[i] = NULL;
}

Test::~Test() { }

void Test::_ready()
{
#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Hello World from GDExtension");
#endif

	//pos = Vector2(0, 100);
	moveForce = Vector2(0, 0);

	if (Engine::get_singleton()->is_editor_hint())
		return;

	playerRb = get_node<RigidBody2D>("/root/Main/Player");
	//playerArea = get_node<Area2D>("/root/Main/Player/Area2D");
	playerArea = get_node<Area2D>("/root/Main/Player");
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
	touchPlayerBt = get_node<TouchScreenButton>("/root/Main/Player/PlayerTouch");

	if (touchPlayerBt == NULL)
		UtilityFunctions::print("touchPlayerBt not found");
	else	//This is because the Editor will try to access it, while loading up.
	{
		touchPlayerBt->connect("pressed", Callable(this, "OnTappingOnPlayer"));
		//touchPlayerBt->connect("released", Callable(this, "OnTappingOnPlayer"));
	}

	deltaY = playerNode->get_position().y;

	//get_node<Test>("/root/Main/Player/Test");	
	//print_tree();
}

void Test::OnBodyEntered(Variant body)
{
#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Collision Detected");
#endif

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
			emit_signal("drop_collected", "Drop Collected");
			//Not Any of this below
			//controllerNode->reparent(NULL);
			//dropCollected->call_deferred("queue_free");
			//dropCollected->set_process(false);				//<=========== not this
		}
	}

	//dropSpawner->dropsSpawned--;
}

void Test::OnTappingOnPlayer() 
{ 
	pressedOnPlayer = !pressedOnPlayer;		//default value is false, so first value will be true
	//UtilityFunctions::print("pressedOnPlayer : ", pressedOnPlayer);
}

void Test::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return;

#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Hello World from GDExtension");
#endif
	if (!playMode)
		return;

	if (moving)
	{
		//pos.x = pos.x + moveSpeed * moveMultX * delta;
		//pos.y = pos.y + moveSpeed * moveMultY * delta;
		//set_position(pos);

		playerRb->apply_impulse(moveForce * moveForceMult, Vector2(0, 0));
#ifdef DEBUG_BLOCK
		UtilityFunctions::print("Player Pos: ", playerNode->get_position().x);
#endif
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
#ifdef DEBUG_BLOCK
				UtilityFunctions::print("Key Pressed");
#endif

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
#ifdef DEBUG_BLOCK
					UtilityFunctions::print("Key Pressed");
#endif
					break;
				}
			}
			else if (inputKey->is_released())
			{
#ifdef DEBUG_BLOCK
				UtilityFunctions::print("Key Released");
#endif

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
#ifdef DEBUG_BLOCK
					UtilityFunctions::print("Key Pressed");
#endif
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
			if (inputTouch != NULL)
			{
				if (inputTouch->is_pressed())
				{
					Vector2 touchPos = inputTouch->get_position();
					deltaX = touchPos.x - playerNode->get_position().x;

					//RayCast code to check if tapped on player
					/*if (touchPlayerBt->is_pressed())
					{
#ifdef DEBUG_BLOCK
						UtilityFunctions::print("Pressed touchPlayerBt");
#endif
						pressedOnPlayer = true;
					}
					else
						UtilityFunctions::print("Not Pressed touchPlayerBt");*/
				}
				else if (inputTouch->is_released())
					pressedOnPlayer = false;
			}
		}

		if (pressedOnPlayer)
		{
			Ref<InputEventScreenDrag> inputDrag = static_cast<Ref<InputEventScreenDrag>>(event);
			if (inputDrag != NULL)
			{
				Vector2 touchPos = inputDrag->get_position();
				float newPosX = touchPos.x - deltaX;
				if (newPosX >= 40.0f && newPosX <= 680.0f)
					playerNode->set_position(Vector2(newPosX, deltaY));
					//return;
				//UtilityFunctions::clampf(newPosX, 40.0f, 680.0f);

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
	ClassDB::bind_method(D_METHOD("SetMoveForceMult" , "speed"), &Test::SetMoveForceMult);
	ClassDB::bind_method(D_METHOD("GetMoveForceMult"), &Test::GetMoveForceMult);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "moveForceMult"), "SetMoveForceMult", "GetMoveForceMult");

	ClassDB::bind_method(D_METHOD("SetTestTex", "tex"), &Test::SetTestTex);
	ClassDB::bind_method(D_METHOD("GetTestTex"), &Test::GetTestTex);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "_testTex", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "SetTestTex", "GetTestTex");

	//ClassDB::bind_method(D_METHOD("SetPlayerRB", "rigidBody"), &SetPlayerRB);
	////ClassDB::bind_method(D_METHOD("SetPlayerRB"), &SetPlayerRB);
	//ClassDB::bind_method(D_METHOD("GetPlayerRB"), &GetPlayerRB);
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "playerRB", PROPERTY_HINT_RESOURCE_TYPE, "RigidBody2D"), "SetPlayerRB", "GetPlayerRB");

	ClassDB::bind_method(D_METHOD("SetPlayMode", "toggleVal"), &Test::SetPlayMode);
	ClassDB::bind_method(D_METHOD("GetPlayMode"), &Test::GetPlayMode);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enablePlayMode"), "SetPlayMode", "GetPlayMode");

	ClassDB::bind_method(D_METHOD("OnBodyEntered", "body"), &Test::OnBodyEntered);
	ClassDB::bind_method(D_METHOD("OnTappingOnPlayer"), &Test::OnTappingOnPlayer);

	ADD_SIGNAL(MethodInfo("drop_collected", PropertyInfo(Variant::STRING, "Message")));
}