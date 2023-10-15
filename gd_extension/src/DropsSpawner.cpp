#define TEST_MODE

#include "DropsSpawner.h"
#include <godot_cpp/classes/engine.hpp>

DropsSpawner::DropsSpawner()
{
	dropsContainer = NULL;
	spawnPos = Vector2(0, 0);
	maxSpawnTime = minSpawnTime = randCallTime = dropToCall = dropCallCount = 0;
	checkDropsCounter = timePassedF = 0.0f;
	dropsSpawned = 0;

	for (int i = 0; i < 10; i++)
	{
		drops[i] = NULL;
		dropsEnableStatus[i] = false;
	}

	spawnTimes = NULL, consecutive = NULL;
	sceneReady = false;
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

#ifdef TEST_MODE
	set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
#endif

	dropsContainer = get_node<Node2D>("/root/Main/WaterDropsContainer");
	if (dropsContainer != NULL)
	{
		spawnPos = dropsContainer->get_position();

		int childCount = dropsContainer->get_child_count();
		for (int i = 0; i < childCount; i++)
		{
			drops[i] = Object::cast_to<Node2D>(dropsContainer->get_child(i));
		}
		
		//UtilityFunctions::print("Trying to remove", dropsContainer->get_child_count());
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

				//UtilityFunctions::print("Successful removal at : ", i);
			}
			//childCount = dropsContainer->get_child_count();
		}
		UtilityFunctions::print("Successful removal, Count : ", dropsContainer->get_child_count());

		/*dropsContainer->remove_child(drops[0]);
		SpawnDrops();*/
		sceneReady = true;
	}

	//spawnTimes = new float(10);
	consecutive = new bool[10] {false, false, false, false, false, true, false, false, false, true};
	spawnTimes = new float[10] {3.5f, 3.3f, 0.7f, 1.5f, 2.8f, 0.0f, 2.0f, 1.0f, 3.3f, 0.0f };

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
	if (checkDropsCounter > 1.0f)
	{
		CheckIfDropsAreBelowGround();
		checkDropsCounter = 0.0f;
	}
}

void DropsSpawner::SpawnDrops()
{
	//UtilityFunctions::print("Spawner Called", dropsContainer->get_child_count());

	/*if (dropsSpawned[dropCalled])
		return;*/
	if (initialSpawnsDone)
	{
		for (int i = 0; i < 10; i++)
		{
			if (!dropsEnableStatus[i])
			{
				dropToCall = i;
				UtilityFunctions::print("Available Drop At : ", i);
				break;
			}
		}
	}

	if (drops[dropToCall] == NULL)
		UtilityFunctions::print("Drop is null");/**/

	int randPosX = (UtilityFunctions::randf() * 620) - 310;
	Vector2 pos = Vector2(randPosX, 0);

	RigidBody2D* dropRB = Object::cast_to<RigidBody2D>(drops[dropToCall]);
	dropRB->call_deferred("set_freeze_enabled", false);
	//dropRB->set_freeze_enabled(false);

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
			//UtilityFunctions::print("initialSpawnsDone : ", initialSpawnsDone);
		}
	}

	//UtilityFunctions::print("Drops Added", dropsContainer->get_child_count());
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

	//UtilityFunctions::print("Drops Remove At : ", dropID);
}

void DropsSpawner::CheckIfDropsAreBelowGround()
{
	//UtilityFunctions::print("Position 0 Y : ", drops[0]->get_position().y);

	//UtilityFunctions::print("Checking if drops are above ground");
	for (int i = 0; i < 10; i++)
		if (dropsEnableStatus[i])
			if (drops[i]->get_position().y > 1200.0f)
			{
				DisableDrop(i, drops[i]);
				UtilityFunctions::print("Below Ground, Drops Remove At : ", i);
			}
}

#pragma region Properties
int DropsSpawner::GetMinSpawnTime() { return minSpawnTime; }
void DropsSpawner::SetMinSpawnTime(int spawnTime) { minSpawnTime = spawnTime; }

int DropsSpawner::GetMaxSpawnTime() { return maxSpawnTime; }
void DropsSpawner::SetMaxSpawnTime(int spawnTime) { maxSpawnTime = spawnTime; }

//RigidBody2D Test::GetPlayerRB() { return playerRb; }
//void Test::SetPlayerRB(RigidBody2D rigidBody) { get_node<RigidBody2D>("PlayerRB_2D"); }			//RigidBody2D rigidBody
#pragma endregion

void DropsSpawner::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("SetMinSpawnTime", "minSpawnTime"), &SetMinSpawnTime);
	ClassDB::bind_method(D_METHOD("GetMinSpawnTime"), &GetMinSpawnTime);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "minSpawnTime"), "SetMinSpawnTime", "GetMinSpawnTime");

	ClassDB::bind_method(D_METHOD("SetMaxSpawnTime", "maxSpawnTime"), &SetMaxSpawnTime);
	ClassDB::bind_method(D_METHOD("GetMaxSpawnTime"), &GetMaxSpawnTime);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "maxSpawnTime"), "SetMaxSpawnTime", "GetMaxSpawnTime");

	ClassDB::bind_method(D_METHOD("SpawnDrops"), &SpawnDrops);
}