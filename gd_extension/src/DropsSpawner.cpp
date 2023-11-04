#define TEST_MODE

#include "DropsSpawner.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/animation_player.hpp>

DropsSpawner::DropsSpawner()
{
	dropsContainer = NULL;
	spawnPos = Vector2(0.0, 0.0);
	maxSpawnTime = minSpawnTime = randCallTime = dropToCall = dropCallCount = 0;
	checkDropsCounter = timePassedF = 0.0f;
	dropsSpawned = 0;

	for (int i = 0; i < 10; i++)
	{
		drops[i] = NULL;
		dropsEnableStatus[i] = false;
	}

	for (int i = 0; i < 5; i++)
		dropsSpawnPos.push_back(Vector2(0.0, 0.0));

	spawnTimes = NULL, consecutive = NULL;
	sceneReady = activateSpawner = scene2Selected = false;
}

DropsSpawner::~DropsSpawner() 
{
	if (spawnTimes != NULL)
		delete spawnTimes;
	if (consecutive != NULL)
		delete consecutive;
}

void DropsSpawner::_ready()
{
	if (Engine::get_singleton()->is_editor_hint())
		return;

	if (!activateSpawner)
		set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);

	dropsContainer = get_node<Node2D>("/root/Main/WaterDropsContainer");
	if (dropsContainer != NULL)
	{
		spawnPos = dropsContainer->get_position();

		int childCount = dropsContainer->get_child_count();
		for (int i = 0; i < childCount; i++)
		{
			drops[i] = Object::cast_to<Node2D>(dropsContainer->get_child(i));
		}

#ifdef DEBUG_BLOCK
		UtilityFunctions::print("Trying to remove", dropsContainer->get_child_count());
#endif
		for (int i = 0; i < childCount; i++)
		{
			if (drops[i] != NULL)
			{
				//UtilityFunctions::print("Successful : ", i);
				//dropsContainer->remove_child(drops[0]);
				dropsContainer->call_deferred("remove_child", drops[i]);

				DropsController* tempDrop = Object::cast_to<DropsController>(drops[i]->get_child(0));
				tempDrop->dropID = i;/**/
				//drops[i]->queue_free();

#ifdef DEBUG_BLOCK
				UtilityFunctions::print("Successful removal at : ", i);
#endif
			}
			//childCount = dropsContainer->get_child_count();
		}
#ifdef DEBUG_BLOCK
		UtilityFunctions::print("Successful removal, Count : ", dropsContainer->get_child_count());
#endif

		/*dropsContainer->remove_child(drops[0]);
		SpawnDrops();*/
		sceneReady = true;
	}

	//spawnTimes = new float(10);

#ifdef TEST_MODE
	consecutive = new bool[10] {false, false, false, false, false, false, false, false, false, false};

	spawnTimes = new float[10];
	for (int i = 0; i < 10; i++)
		spawnTimes[i] = (UtilityFunctions::randf() * 1.0f) + 2.0f;		//(2s-3s)
#else
	consecutive = new bool[10] {false, false, false, false, false, true, false, false, false, true};
	spawnTimes = new float[10] {3.5f, 3.3f, 0.7f, 1.5f, 2.8f, 0.0f, 2.0f, 1.0f, 3.3f, 0.0f };
#endif

	minSpawnTime = 2;
	maxSpawnTime = 5;
	randCallTime = (UtilityFunctions::randf() * (maxSpawnTime - minSpawnTime)) + minSpawnTime;
}

void DropsSpawner::_process(double delta)
{
	if (Engine::get_singleton()->is_editor_hint())
		return;

	if (!sceneReady)
		return;

	/*if (timePassed < 160)
	{
		timePassed++;
		if (spawnCalled)
			UtilityFunctions::print("Time Passed : ", timePassed);
	}*/

	if (dropsSpawned < 10)
	{
		timePassedF += delta;

		//if (timePassed > randCallTime && !spawnCalled)
		if (timePassedF > randCallTime)
		{
			timePassedF = 0;

			//CAse 1
			/*randCallTime = (UtilityFunctions::randf() * (maxSpawnTime - minSpawnTime)) + minSpawnTime;
			UtilityFunctions::print("Spawning Drop, randCallTime : ", randCallTime);*/

			//Case 2
			randCallTime = spawnTimes[dropCallCount];
			dropCallCount++;
			if (dropCallCount > 9)
				dropCallCount = 0;

			SpawnDrops();
			//spawnCalled = true;
		}
	}

	checkDropsCounter += delta;
	if (checkDropsCounter > 0.5f)
	{
		CheckIfDropsAreBelowGround();
		checkDropsCounter = 0.0f;
	}
}

void DropsSpawner::SpawnDrops()
{
#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Spawner Called", dropsContainer->get_child_count());
#endif

	/*if (dropsSpawned[dropCalled])
		return;*/
	if (initialSpawnsDone)
	{
		for (int i = 0; i < 10; i++)
		{
			if (!dropsEnableStatus[i])
			{
				dropToCall = i;
#ifdef DEBUG_BLOCK
				UtilityFunctions::print("Available Drop At : ", i);
#endif
				break;
			}
		}
	}

	if (drops[dropToCall] == NULL)
		UtilityFunctions::print("Error!! Drop is null");/**/

#ifdef RANDOM_POS
	int randPosX = (UtilityFunctions::randf() * 620) - 310;
	Vector2 pos = Vector2(randPosX, 0);
#else
	int randSpawnArrIndex;
	if (scene2Selected)
		randSpawnArrIndex = (UtilityFunctions::randf() * 4) + 4;	//Starts from index 4
	else
		randSpawnArrIndex = UtilityFunctions::randf() * 4;
	Vector2 pos = dropsSpawnPos[randSpawnArrIndex];
#endif // RANDOM_POS

	//Enable Rigidbody
	RigidBody2D* dropRB = Object::cast_to<RigidBody2D>(drops[dropToCall]);
	dropRB->call_deferred("set_freeze_enabled", false);
	//dropRB->set_freeze_enabled(false);

	//Play Start Animation
	AnimationPlayer* dropAnimation = Object::cast_to<AnimationPlayer>(drops[dropToCall]->get_child(2));
	dropAnimation->play("Grow_Drop");

	dropsContainer->add_child(drops[dropToCall]);
	//dropsContainer->call_deferred("add_child", drops[dropToCall]);
	drops[dropToCall]->set_position(pos);

	//add_child(drops[dropCalled]);

	dropsEnableStatus[dropToCall] = true;

	dropsSpawned++;
	/*if (initialSpawnsDone)
		return;*/
	//All the drops have been spawned
	if (!initialSpawnsDone)
	{
		dropToCall++;
		if (dropToCall > 9)
		{
			initialSpawnsDone = true;
#ifdef DEBUG_BLOCK
			UtilityFunctions::print("initialSpawnsDone : ", initialSpawnsDone);
#endif
		}
	}

#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Drops Added", dropsContainer->get_child_count());
#endif
}

void DropsSpawner::DisableDrop(unsigned short dropID, Node2D* drop)
{
	//Searching for the particular drop inside the drops array
	/*int childCount = dropsContainer->get_child_count();
	Node* tempDrop = NULL;
	for (int i = 0; i < childCount; i++)
	{
		tempDrop = dropsContainer->get_child(i);
		DropsController* controllerNode = Object::cast_to<DropsController>(tempDrop->get_child(0));
		if (controllerNode->dropID == dropID)
			break;
	}

	if (tempDrop == NULL)
		return;
	dropsContainer->call_deferred("remove_child", tempDrop);*/

	RigidBody2D* dropRB = Object::cast_to<RigidBody2D>(drop);
	//dropRB->set_freeze_enabled(true);
	//dropRB->set_freeze_mode(RigidBody2D::FREEZE_MODE_KINEMATIC);
	dropRB->set_linear_velocity(Vector2(0, 0));

	dropRB->call_deferred("set_freeze_enabled", true);
	dropsContainer->call_deferred("remove_child", drop);
	dropsEnableStatus[dropID] = false;
	dropsSpawned--;

#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Drops Remove At : ", dropID);
#endif
}

void DropsSpawner::CheckIfDropsAreBelowGround()
{
#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Position 0 Y : ", drops[0]->get_position().y);
#endif

#ifdef DEBUG_BLOCK
	UtilityFunctions::print("Checking if drops are above ground");
#endif
	for (int i = 0; i < 10; i++)
		if (dropsEnableStatus[i])
			if (drops[i]->get_position().y > 1200.0f)
			{
				DisableDrop(i, drops[i]);
				emit_signal("drop_missed", "Drop Missed");
#ifdef DEBUG_BLOCK
				UtilityFunctions::print("Below Ground, Drops Remove At : ", i);
#endif
			}
}

#pragma region Properties
int DropsSpawner::GetMinSpawnTime() { return minSpawnTime; }
void DropsSpawner::SetMinSpawnTime(int spawnTime) { minSpawnTime = spawnTime; }

int DropsSpawner::GetMaxSpawnTime() { return maxSpawnTime; }
void DropsSpawner::SetMaxSpawnTime(int spawnTime) { maxSpawnTime = spawnTime; }

PackedVector2Array DropsSpawner::GetDropSpawnPos1() { return dropsSpawnPos; }
void DropsSpawner::SetDropSpawnPos1(PackedVector2Array _dropsSpawnPos) { dropsSpawnPos = _dropsSpawnPos; }

bool DropsSpawner::GetActivateSpawner() { return activateSpawner; }
void DropsSpawner::SetActivateSpawner(bool status) { activateSpawner = status; }

bool DropsSpawner::GetScene2Selected() { return scene2Selected; }
void DropsSpawner::SetScene2Selected(bool status) { scene2Selected = status; }

//RigidBody2D Test::GetPlayerRB() { return playerRb; }
//void Test::SetPlayerRB(RigidBody2D rigidBody) { get_node<RigidBody2D>("PlayerRB_2D"); }			//RigidBody2D rigidBody
#pragma endregion

void DropsSpawner::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("SetMinSpawnTime", "minSpawnTime"), &DropsSpawner::SetMinSpawnTime);
	ClassDB::bind_method(D_METHOD("GetMinSpawnTime"), &DropsSpawner::GetMinSpawnTime);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "minSpawnTime"), "SetMinSpawnTime", "GetMinSpawnTime");

	ClassDB::bind_method(D_METHOD("SetMaxSpawnTime", "maxSpawnTime"), &DropsSpawner::SetMaxSpawnTime);
	ClassDB::bind_method(D_METHOD("GetMaxSpawnTime"), &DropsSpawner::GetMaxSpawnTime);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "maxSpawnTime"), "SetMaxSpawnTime", "GetMaxSpawnTime");

	ClassDB::bind_method(D_METHOD("SetDropSpawnPos1", "_dropsSpawnPos"), &DropsSpawner::SetDropSpawnPos1);
	ClassDB::bind_method(D_METHOD("GetDropSpawnPos1"), &DropsSpawner::GetDropSpawnPos1);
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "dropsSpawnPos"), "SetDropSpawnPos1", "GetDropSpawnPos1");

	ClassDB::bind_method(D_METHOD("SetActivateSpawner", "status"), &DropsSpawner::SetActivateSpawner);
	ClassDB::bind_method(D_METHOD("GetActivateSpawner"), &DropsSpawner::GetActivateSpawner);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "activateSpawner"), "SetActivateSpawner", "GetActivateSpawner");

	ClassDB::bind_method(D_METHOD("SetScene2Selected", "status"), &DropsSpawner::SetScene2Selected);
	ClassDB::bind_method(D_METHOD("GetScene2Selected"), &DropsSpawner::GetScene2Selected);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "scene2Selected"), "SetScene2Selected", "GetScene2Selected");

	ClassDB::bind_method(D_METHOD("SpawnDrops"), &DropsSpawner::SpawnDrops);

	ADD_SIGNAL(MethodInfo("drop_missed", PropertyInfo(Variant::STRING, "Message")));
}