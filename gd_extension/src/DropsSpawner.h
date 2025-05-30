#ifndef DROPSSPAWNER_H
#define DROPSSPAWNER_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/audio_stream_player2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>

#include "DropsController.h"

using namespace godot;

class DropsController;
class DropsSpawner : public Node
{
	GDCLASS(DropsSpawner, Node);

public:
	short dropsSpawned;

	void _ready();
	void _process(double delta);

	void SetMaxSpawnTime(int spawnTime);
	int GetMaxSpawnTime();

	void SetMinSpawnTime(int spawnTime);
	int GetMinSpawnTime();

	void SetActivateSpawner(bool status);
	bool GetActivateSpawner();

	void SetScene2Selected(bool status);
	bool GetScene2Selected();

	void SetDropSpawnPos1(PackedVector2Array dropsSpawnPos);
	PackedVector2Array GetDropSpawnPos1();

	void DisableDrop(unsigned short dropID, Node2D* drop);

protected:
	static void _bind_methods();

private:
	Node2D* dropsContainer;
	Node2D* drops[10];

	bool sceneReady, initialSpawnsDone, activateSpawner, scene2Selected;

	Vector2 spawnPos;
	bool dropsEnableStatus[10];

	float* spawnTimes;
	bool* consecutive;
	unsigned short maxSpawnTime, minSpawnTime, randCallTime, dropToCall, dropCallCount;

	float timePassedF;
	void SpawnDrops();

	float checkDropsCounter;
	void CheckIfDropsAreBelowGround();

	PackedVector2Array dropsSpawnPos;

	DropsSpawner();
	~DropsSpawner();
};

#endif